/* Read-only USB hub/configuration inspection; no reset, SET requests or serial. */
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <initguid.h>
#include <setupapi.h>
#include <winioctl.h>
#include <usbioctl.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hid_endpoint { unsigned int interface_number, address, packet, interval; };
static unsigned int le16(const unsigned char *p) { return p[0] | ((unsigned int)p[1] << 8); }
static int parse_config(const unsigned char *p, size_t size, struct hid_endpoint *out)
{
    if(size<9 || p[0]!=9 || p[1]!=2 || le16(p+2)!=size || !p[4] || !p[5])return -1;
    unsigned int matches=0,interface_number=0;
    int hid=0;
    for(size_t pos=9;pos<size;) {
        if(size-pos<2 || p[pos]<2 || p[pos]>size-pos)return -1;
        unsigned int length=p[pos],type=p[pos+1];
        if(type==4) {
            if(length<9)return -1;
            interface_number=p[pos+2];
            hid=p[pos+3]==0 && p[pos+5]==3;
        } else if(type==5) {
            if(length<7)return -1;
            if(hid && (p[pos+2]&128) && (p[pos+3]&3)==3) {
                unsigned int address=p[pos+2],packet=le16(p+pos+4),interval=p[pos+6];
                if((address&0x70) || !(address&15) || !packet || packet>64 || !interval)return -1;
                matches++;
                out->interface_number=interface_number;out->address=address;
                out->packet=packet;out->interval=interval;
            }
        }
        pos+=length;
    }
    return matches==1 ? 0:-1;
}
#define CHECK(c) do { if (!(c)) { fprintf(stderr,"Self-test failed line %d\n",__LINE__); return 1; } } while(0)
static int self_test(void)
{
    unsigned char fixture[] = {9,2,25,0,1,1,0,128,50,9,4,2,0,1,3,0,0,0,7,5,129,3,64,0,1};
    struct hid_endpoint e;
    CHECK(parse_config(fixture,sizeof(fixture),&e)==0);
    CHECK(e.interface_number==2 && e.address==129 && e.packet==64 && e.interval==1);
    for(size_t n=0;n<sizeof(fixture);n++) CHECK(parse_config(fixture,n,&e)!=0);
    unsigned char bad[sizeof(fixture)];
    const unsigned int positions[]={0,2,9,14,18,20,21,22,24};
    const unsigned char values[]={0,26,0,2,6,1,2,0,0};
    for(size_t i=0;i<sizeof(positions)/sizeof(positions[0]);i++) {
        memcpy(bad,fixture,sizeof(bad));bad[positions[i]]=values[i];CHECK(parse_config(bad,sizeof(bad),&e)!=0);
    }
    unsigned char duplicate[32];memcpy(duplicate,fixture,sizeof(fixture));memcpy(duplicate+25,fixture+18,7);duplicate[2]=32;
    CHECK(parse_config(duplicate,sizeof(duplicate),&e)!=0);
    puts("PASS: descriptor bounds, HID selection, interval and ambiguity guards; no hardware access");
    return 0;
}

static int get_config(HANDLE hub, ULONG port, USHORT length, unsigned char *output)
{
    DWORD bytes=(DWORD)sizeof(USB_DESCRIPTOR_REQUEST)+length,got=0;
    USB_DESCRIPTOR_REQUEST *request=(USB_DESCRIPTOR_REQUEST *)calloc(1,bytes);
    if(!request)return -1;
    request->ConnectionIndex=port;
    request->SetupPacket.wValue=USB_CONFIGURATION_DESCRIPTOR_TYPE<<8;
    request->SetupPacket.wLength=length;
    BOOL ok=DeviceIoControl(hub,IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,request,bytes,request,bytes,&got,NULL);
    if(ok && got==bytes)memcpy(output,request->Data,length);
    free(request);
    return ok && got==bytes ? 0:-1;
}

static int inspect_target(HANDLE hub,ULONG port,const USB_NODE_CONNECTION_INFORMATION_EX *info,struct hid_endpoint *endpoint)
{
    unsigned char descriptor[4096];
    if(info->Speed!=UsbFullSpeed || info->CurrentConfigurationValue==0)return -1;
    if(get_config(hub,port,9,descriptor))return -1;
    unsigned int length=le16(descriptor+2);
    if(length<9 || length>sizeof(descriptor) || get_config(hub,port,(USHORT)length,descriptor))return -1;
    if(descriptor[5]!=info->CurrentConfigurationValue)return -1;
    if(parse_config(descriptor,length,endpoint))return -1;
    unsigned int matching=0;
    if(info->NumberOfOpenPipes>32)return -1;
    for(ULONG i=0;i<info->NumberOfOpenPipes;i++) {
        const USB_ENDPOINT_DESCRIPTOR *pipe=&info->PipeList[i].EndpointDescriptor;
        if(pipe->bEndpointAddress==endpoint->address && (pipe->bmAttributes&3)==3 &&
           pipe->bInterval==endpoint->interval && pipe->wMaxPacketSize==endpoint->packet)matching++;
    }
    return matching==1 ? 0:-1;
}

int main(int argc,char **argv)
{
    if(argc==2 && strcmp(argv[1],"--self-test")==0)return self_test();
    if(argc!=1){fputs("Usage: inspect_usb_endpoint [--self-test]\n",stderr);return 2;}
    HDEVINFO list=SetupDiGetClassDevsW(&GUID_DEVINTERFACE_USB_HUB,NULL,NULL,DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
    if(list==INVALID_HANDLE_VALUE){fputs("Hub enumeration unavailable\n",stderr);return 2;}
    unsigned int hubs=0,inaccessible=0,matches=0,invalid=0;
    struct hid_endpoint endpoint={0};
    for(DWORD index=0;;index++) {
        SP_DEVICE_INTERFACE_DATA item={0};item.cbSize=sizeof(item);
        if(!SetupDiEnumDeviceInterfaces(list,NULL,&GUID_DEVINTERFACE_USB_HUB,index,&item)) {
            if(GetLastError()!=ERROR_NO_MORE_ITEMS)inaccessible++;
            break;
        }
        hubs++;
        DWORD size=0;
        SetupDiGetDeviceInterfaceDetailW(list,&item,NULL,0,&size,NULL);
        if(size<sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W) || size>65536){inaccessible++;continue;}
        SP_DEVICE_INTERFACE_DETAIL_DATA_W *detail=(SP_DEVICE_INTERFACE_DETAIL_DATA_W *)calloc(1,size);
        if(!detail){inaccessible++;continue;}
        detail->cbSize=sizeof(*detail);
        BOOL ok=SetupDiGetDeviceInterfaceDetailW(list,&item,detail,size,NULL,NULL);
        HANDLE hub=ok?CreateFileW(detail->DevicePath,0,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL):INVALID_HANDLE_VALUE;
        free(detail);
        if(hub==INVALID_HANDLE_VALUE){inaccessible++;continue;}
        USB_NODE_INFORMATION node={0};node.NodeType=UsbHub;DWORD got=0;
        if(!DeviceIoControl(hub,IOCTL_USB_GET_NODE_INFORMATION,&node,sizeof(node),&node,sizeof(node),&got,NULL) || got<sizeof(node)) {inaccessible++;CloseHandle(hub);continue;}
        DWORD bytes=(DWORD)(sizeof(USB_NODE_CONNECTION_INFORMATION_EX)+32*sizeof(USB_PIPE_INFO));
        USB_NODE_CONNECTION_INFORMATION_EX *info=(USB_NODE_CONNECTION_INFORMATION_EX *)calloc(1,bytes);
        if(!info){inaccessible++;CloseHandle(hub);continue;}
        for(ULONG port=1;port<=node.u.HubInformation.HubDescriptor.bNumberOfPorts;port++) {
            memset(info,0,bytes);info->ConnectionIndex=port;got=0;
            if(!DeviceIoControl(hub,IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX,info,bytes,info,bytes,&got,NULL) || got<sizeof(*info)){inaccessible++;continue;}
            if(info->ConnectionStatus!=DeviceConnected || info->DeviceDescriptor.idVendor!=0x2fe3 || info->DeviceDescriptor.idProduct!=0x0008)continue;
            matches++;
            if(info->NumberOfOpenPipes>32 || got<sizeof(*info)+info->NumberOfOpenPipes*sizeof(USB_PIPE_INFO) || inspect_target(hub,port,info,&endpoint))invalid++;
        }
        free(info);CloseHandle(hub);
    }
    SetupDiDestroyDeviceInfoList(list);
    /* shortcut: verify only full-speed bInterval1; add Windows period mapping for other intervals. */
    if(!hubs || inaccessible || matches!=1 || invalid || endpoint.interval!=1) {
        fprintf(stderr,"Incomplete/ambiguous USB inspection: hubs=%u inaccessible=%u targets=%u invalid=%u bInterval=%u\n",hubs,inaccessible,matches,invalid,endpoint.interval);return 2;
    }
    printf("{\"vid\":%u,\"pid\":8,\"speed\":\"full\",\"hid_interface\":%u,\"interrupt_in_endpoint\":%u,\"max_packet_bytes\":%u,\"interval_ms\":%u,\"requested_max_poll_hz\":%.3f,\"active_pipe_matches\":true,\"hubs_checked\":%u,\"scope\":\"Descriptor/configuration ceiling only; not measured bus cadence or fresh sensor report rate\"}\n",0x2fe3u,endpoint.interface_number,endpoint.address,endpoint.packet,endpoint.interval,1000.0/endpoint.interval,hubs);
    return 0;
}
