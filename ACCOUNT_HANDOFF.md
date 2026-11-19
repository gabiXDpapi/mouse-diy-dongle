# Continue under another account

The project is paused. Normal v14 remains installed and usable. The30-minute
user-reported gameplay check passed; the next4h use window has not started.

## Same PC and same Windows login

1. Keep this entire project folder in place, including ignored local files:
   C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE
2. Open the app profile menu, log out and sign in with the intended account.
   Complete the browser flow with that account. Do not delete project files or
   clear credentials manually as part of the project handoff.
3. Reopen this local folder in Codex and start a new chat. Grant access to the
   folder/tools when requested. Reconnect any integrations only if needed.
4. Paste the prompt below, or the contents of RESUME_PROMPT.md. The new chat can
   recover the project state from files without importing this conversation.

```text
Continue my project in C:\Users\Gabriel\OneDrive\Documents\MOUSE DIY DONGLE.
Read RESUME_PROMPT.md, PROJECT_CONTEXT.md (LATEST HANDOFF / NEXT), README.md,
research/RADIO_ANALYSIS.md and the gameplay qualification plan.
Normal v14 is installed; the30-minute gameplay check passed. The4h trial has
NOT started. Ask for fresh readiness; do not rebuild/reflash/repeat baselines.
Keep private captures/addresses/binaries ignored and pause for physical steps.
```

The new account needs its own access to the relevant Codex features. Do not rely
on chat history, memories, settings or subscription moving with the local files.
This procedure carries project state, not the OpenAI account itself.

## Different computer or Windows login

Copy the project privately, including build-pairing-analysis and its ignored RF
parameters, tested UF2s, captures and diaries. A Git clone alone excludes these.
Never add the private data to Git merely to transfer it. Update the project path
in the handoff if needed and verify filesystem permissions on the destination.

The existing Zephyr workspace is outside the repo at
C:\Users\Gabriel\zephyrproject. Other installed compiler/SDK tools also live
outside the repo. Follow the recorded environment setup and verify/recreate it
on the new computer; virtual environments are not assumed portable. Preserve the
exact normal-v14 rollback image/hash from RESUME_PROMPT.md. No auth credentials
need copying. Reauthenticate Git/integrations only as required on the destination.

Official sign-in/sign-out guidance:
https://learn.chatgpt.com/docs/auth
Account separation/reference export limitations:
https://help.openai.com/en/articles/9106926-transfer-exported-conversations-between-chatgpt-accounts
