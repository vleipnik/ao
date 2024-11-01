// aos-sqlite/wasm/didkit/didkit_loader.js
import init, * as didkit from './didkit.js';

async function initializeDIDKit() {
    await init();
    console.log("DIDKit WASM Module Loaded");
    // Export DIDKit functions globally or as needed for Lua integration
    globalThis.didkit = didkit;
}

initializeDIDKit();

console.log('======= VIC ====== DIDKIT LOADER')
