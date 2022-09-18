const addon = require('bindings')('node_win');

namespace Windows {
    export class Input {
        public getTimeSinceLastInput() {
            return addon.GetTimeSinceLastInput();
        }
    }

    export class ServiceManager {
        public install(serviceName: string, displayName: string, fileName: string): boolean {
            return addon.Service_Install(serviceName, displayName, fileName);
        }

        public uninstall(serviceName: string): boolean {
            return addon.Service_Uninstall(serviceName);
        }

        public isInstalled(serviceName: string): boolean {
            return addon.Service_IsInstalled(serviceName);
        }

        public start(serviceName: string): boolean {
            return addon.Service_Start(serviceName);
        }

        public stop(serviceName: string): boolean {
            return addon.Service_Stop(serviceName);
        }
    }
}

export = Windows;