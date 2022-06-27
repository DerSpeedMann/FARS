import { Component } from '@angular/core';
import { CdkDragDrop, moveItemInArray, transferArrayItem } from '@angular/cdk/drag-drop';
import { FormBuilder, FormControl, Validators } from '@angular/forms';
import { Phase } from './models/phase';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'FARS';

  items = ['Carrots', 'Tomatoes', 'Onions', 'Apples', 'Avocados', 'Bananas', 'Cucumbers'];

  phases: Phase[] = [new Phase("Preprocessing"), new Phase("Extraction"), new Phase("Matching")];

  // TODO: create template for details
  checkboxControl = new FormControl(false);
  selectControl = new FormControl('primary');
  inputNumControl = new FormControl(16, Validators.min(10));
  options = this._formBuilder.group({
    checkbox: this.checkboxControl,
    select: this.selectControl,
    inputNum: this.inputNumControl,
  });

  constructor(private _formBuilder: FormBuilder) {}
  // template end

  drop(event: CdkDragDrop<string[]>) {
    if (event.previousContainer === event.container) {
      moveItemInArray(event.container.data, event.previousIndex, event.currentIndex);
    } else {
      transferArrayItem(
        event.previousContainer.data,
        event.container.data,
        event.previousIndex,
        event.currentIndex,
      );
    }
  }
}
