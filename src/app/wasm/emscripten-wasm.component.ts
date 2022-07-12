import { AfterViewInit, Directive } from "@angular/core";
import { loadScript } from "./tools";
import { environment } from "../../environments/environment";

type EmscriptenModuleDecorator<M extends EmscriptenModule> = (module: M) => void;

const noopModuleDecorator = (mod: EmscriptenModule) => mod;

@Directive()
export abstract class EmscriptenWasmComponent<M extends EmscriptenModule = EmscriptenModule> implements AfterViewInit {
  private resolvedModule!: M;
  protected moduleDecorator!: EmscriptenModuleDecorator<M>;
  private moduleExportName: string;
  private wasmJavaScriptLoader: string;

  protected constructor(moduleExportName: string, wasmJavaScriptLoader: string) {
    this.moduleExportName = moduleExportName;
    this.wasmJavaScriptLoader = wasmJavaScriptLoader;
  }

  get module(): M {
    return this.resolvedModule;
  }

  ngAfterViewInit(): void {
    this.resolveModule();
  }

  protected resolveModule(): void {
    loadScript(this.moduleExportName, `${environment.wasmAssetsPath}/${this.wasmJavaScriptLoader}`)
      .then(() => {
        const module = <M>{
          locateFile: (file: string) => {
            return `${environment.wasmAssetsPath}/${file}`;
          },
        };
        const moduleDecorator: EmscriptenModuleDecorator<M> = this.moduleDecorator || noopModuleDecorator;
        moduleDecorator(module);

        return window[this.moduleExportName as keyof typeof window](module);
      })
      .then((mod) => {
        this.resolvedModule = mod;
      });
  }
}
