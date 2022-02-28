const addon = require('bindings')('node_win');

namespace Windows {
    export class Stream {
        public isReady(ip: string): boolean {
            return addon.isAvailable(ip);
        }
    }
}


export = Windows;