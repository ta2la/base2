#pragma once

//=============================================================================
// CmdBridge — unified entry point for "external command in / output broadcast".
// One init() call, transport selected by build:
//   Native (any non-WASM):  WsServerLite on ws://127.0.0.1:12345 (existing).
//   WASM/Emscripten:        EM_JS bridge — JS calls craseExec(cmd) → CMD_SYS;
//                           every command's args mirrored into window.craseLog.
//=============================================================================
class CmdBridge {
public:
    static void init();
};
