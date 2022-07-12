import { Component, ViewChild, ElementRef, NgZone } from "@angular/core";
import { HttpClient } from "@angular/common/http";
import { EmscriptenWasmComponent } from "../emscripten-wasm.component";

const getFileName = (filePath: string) => filePath.split("/").reverse()[0];

const allowedMimeTypes = ["image/bmp", "image/x-windows-bmp", "image/jpeg", "image/pjpeg", "image/png"];

const defaultImage = "assets/img/3d-cube/angular.png";

const requestFullscreen =
  document.documentElement.requestFullscreen ||
  document.documentElement["webkitRequestFullscreen" as keyof typeof document.documentElement] ||
  document.documentElement["msRequestFullscreen" as keyof typeof document.documentElement] ||
  document.documentElement["mozRequestFullScreen" as keyof typeof document.documentElement];

@Component({
  selector: 'app-3d-cube',
  templateUrl: "./3d-cube.component.html",
  styleUrls: ["./3d-cube.component.css"],
})
export class Wasm3dCubeComponent extends EmscriptenWasmComponent {
  @ViewChild("canvas") canvas: ElementRef | undefined;
  predefinedImages: string[];
  error: string | undefined; 
  fileUploadAccept: string;
  supportsFullscreen: boolean;

  constructor(private httpClient: HttpClient, private ngZone: NgZone) {
    super("Cube3dModule", "3d-cube.js");

    this.supportsFullscreen = !!requestFullscreen;
    this.fileUploadAccept = allowedMimeTypes.join(",");
    this.predefinedImages = [defaultImage, "assets/img/3d-cube/cat.png", "assets/img/3d-cube/embroidery.png"];

    this.moduleDecorator = (mod) => {
      mod.arguments = [getFileName(defaultImage)];
      mod.preRun = [
        () => {
            mod.FS_createPreloadedFile 
            ? mod.FS_createPreloadedFile("/", getFileName(defaultImage), defaultImage, true)
            : console.error("FS_createPreloadedFile was undefined");
        },
      ];
      if(this.canvas){
        mod.canvas = <HTMLCanvasElement>this.canvas.nativeElement;
        mod.printErr = (what: string) => {
          if (!what.startsWith("WARNING")) {
            this.ngZone.run(() => (this.error = what));
          }
        };
      }
    };
  }

  toggleFullscreen() {
    if (requestFullscreen && this.canvas) {
      requestFullscreen.bind(this.canvas.nativeElement)();
    }
  }

  selectPredefinedImage(index: number) {
    this.error = undefined;

    const imageUrl: string = this.predefinedImages[index];
    this.httpClient
      .get(imageUrl, { responseType: "arraybuffer" })
      .subscribe((imageBytes) => this.setTexture(getFileName(imageUrl), new Uint8Array(imageBytes)));
  }

  onFileUploaded(event: any) {
    if(!event.target.files.length) return;

    const file:File = event.target.files[0];
    this.error = undefined;

    if (allowedMimeTypes.indexOf(file.type) < 0) {
      this.error = `Unsupported mime type ${file.type}`;
      return;
    }

    const fileName = file.name;

    const reader = new FileReader();
    reader.onload = () => {
      const inputArray = new Uint8Array(<ArrayBuffer>reader.result);
      this.setTexture(fileName, inputArray);
    };
    reader.readAsArrayBuffer(file);
  }

  private setTexture(fileName: string, inputArray: Uint8Array) {
    const isDefaultImage = fileName === getFileName(defaultImage);

    // Default image is always there
    if (!isDefaultImage) {
      this.module.FS_createDataFile
      ? this.module.FS_createDataFile("/", fileName, inputArray, true)
      : console.error("FS_createDataFile was undefined");
    }

    this.module.ccall 
    ? this.module.ccall("set_texture", "void", ["string"], [fileName])
    : console.error("module ccall was undefined");

    // Delete the file afterwards to free memory
    if (!isDefaultImage) {
      this.module.FS_unlink
      ? this.module.FS_unlink(fileName)
      : console.error("module FS_unlink was undefined");
    }
  }
}
