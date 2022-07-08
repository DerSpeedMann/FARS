import { Component, EventEmitter, OnInit, Output } from '@angular/core';

@Component({
  selector: 'app-upload-image',
  templateUrl: './upload-image.component.html',
  styleUrls: ['./upload-image.component.css']
})
export class UploadImageComponent implements OnInit {

  ngOnInit(): void {
  }
  @Output() fileOutput = new EventEmitter<{ file: File }>();
  
  requiredFileType: string = "jpg";
  
  file:File|undefined;
  url = '';

  onFileSelected(event: any) {
    var reader = new FileReader();

    const file:File = event.target.files[0];
    console.log(file.type);
    reader.onload = (event: any) => {
      if (file) {
        this.file = file;
        this.fileOutput.emit({file: this.file})
      }
      this.url = event.target.result;
    };

    reader.onerror = (event: any) => {
      console.log("File could not be read: " + event.target.error.code);
    };

    reader.readAsDataURL(event.target.files[0]);
  }
}
