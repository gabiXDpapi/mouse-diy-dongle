// Offline execution of existing private vendor source; no real HID/native addon or app DB.
const fs = require('fs');
const path = require('path');
const vm = require('vm');
const assert = require('assert').strict;
const crypto = require('crypto');
const privateRoot = path.resolve(__dirname, '../build-pairing-analysis');
const interop = path.join(privateRoot, 'core-static/src/electron-process/renderer-interop');
const sourcePath = path.join(interop, 'protocol/device/mouse/ModelOSeries.js');
const source = fs.readFileSync(sourcePath, 'utf8');
const record = fs.readFileSync(path.join(interop, 'database/SupportDevice.DB'), 'utf8')
    .split(/\r?\n/).filter(Boolean).map(line => JSON.parse(line))
    .find(device => device.devicename === 'SERIES ONE PRO WIRELESS');
assert(record && record.routerID === 'ModelOSeries' && record.LEDSetting === false);
const requests = [], logs = [];
let lastSet, gets = 0, timer;
const hid = {
    SetFeatureReport(deviceId, reportId, length, packet) {
        assert(deviceId === 1 && reportId === 0 && length === 65 && packet.length === 65);
        assert(packet[0] === 0 && packet[6] !== 0xb0);
        lastSet = Buffer.from(packet);
        requests.push({ target: packet[3], size: packet[4], group: packet[5], operation: packet[6], hex: packet.toString('hex') });
        assert(requests.length < 128, 'Unexpected startup loop');
        return 65;
    },
    GetFeatureReport(deviceId, reportId, length) {
        assert(deviceId === 1 && reportId === 0 && length === 65 && lastSet);
        gets++;
        const reply = Buffer.from(lastSet.subarray(1)); // Installed binding strips the ID byte.
        reply[0] = 0xa1;
        if (lastSet[5] === 0 && lastSet[6] === 0x81) { reply[6] = 1; reply[7] = reply[8] = reply[9] = 0; }
        return reply;
    }
};
class MouseStub {
    emit() {}
    setProfileToDevice(device, callback) { callback(); }
    hexToRgb(text) {
        const value = parseInt(text.replace('#',''),16);
        return {color:{R:(value>>16)&255,G:(value>>8)&255,B:value&255}};
    }
}
const appDbStub = {getInstance: () => ({})};
const sandbox = {Buffer, module:{exports:{}}, setTimeout: callback => setTimeout(callback,0),
    require(name) {
        switch (name) {
        case '../../../others/env': return {BuiltType:0,log:(...args)=>logs.push(args)};
        case './Mouse': return MouseStub;
        case '../../../others/EventVariable': return {EventTypes:{}};
        case '../../../others/SupportData': return {};
        case '../../../dbapi/AppDB': return appDbStub;
        case './ModelO2ProPairing': return {getInstance:()=>({})};
        default: throw new Error(`Unexpected dependency refused: ${name}`);
        }
    }
};
vm.runInNewContext(source,sandbox,{filename:'private-ModelOSeries.js',timeout:1000});
const model = new sandbox.module.exports(hid);
const device = {BaseInfo:{...record,SN:'0x258A0x2018',DeviceId:1,StateID:1},
    deviceData:{profileindex:1,profile:JSON.parse(JSON.stringify(record.defaultProfile))}};
async function main() {
    await Promise.race([
        (async()=>{
            await new Promise(resolve => model.InitialDevice(device,0,resolve));
            await new Promise(resolve => model.ReadFWVersion(device,0,resolve));
        })(),
        new Promise((_,reject) => {timer=setTimeout(()=>reject(new Error('Mock startup did not finish')),5000);})
    ]).finally(()=>clearTimeout(timer));
    const errors=logs.filter(line=>line.some(value=>String(value).includes('Error:')));
    assert(!errors.length,JSON.stringify(errors));
    assert(requests.length === 42,`Unexpected factory startup shape: ${requests.length} SETs`);
    assert(requests.filter(packet=>packet.group===3 && packet.operation===0).length===24);
    assert(requests.filter(packet=>packet.target===0 && packet.group===0 && packet.operation===0x81).length===1);
    const counts={};
    for (const packet of requests) {
        const key=[packet.target,packet.group,packet.operation,packet.size].map(value=>value.toString(16).padStart(2,'0')).join('/');
        counts[key]=(counts[key]||0)+1;
    }
    const output=path.join(privateRoot,`core-startup-trace-${Date.now()}.json`);
    fs.writeFileSync(output,JSON.stringify({scope:'OFFLINE MOCK factory defaults only; not hardware responses or user settings',
        sourceSha256:crypto.createHash('sha256').update(source).digest('hex'),sets:requests.length,gets,counts,requests},null,2));
    console.log(JSON.stringify({sets:requests.length,gets,counts,privateResult:output},null,2));
}
main().catch(error=>{console.error(error.message);process.exitCode=1;});
