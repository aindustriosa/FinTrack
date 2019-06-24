import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormGroup } from '@angular/forms';
import { UploadService } from '../upload.service';

@Component({
  selector: 'app-formdata-upload',
  templateUrl: './formdata-upload.component.html',
  styleUrls: ['./formdata-upload.component.css']
})
export class FormdataUploadComponent implements OnInit {

  constructor(private formBuilder: FormBuilder, private uploadService: UploadService) { }

  form: FormGroup;
  error: string;
  userId: number = 1;
  uploadResponse = { status: '', message: '' };

  ngOnInit() {
    this.form = this.formBuilder.group({
      filename: [''],
      vessel: ['']
    });
  }

  onFileChange(event) {
    if (event.target.files.length > 0) {
      const file = event.target.files[0];
      this.form.get('filename').setValue(file);
    }
  }

  onSubmit() {
    const formData = new FormData();

    formData.append('vessel', this.form.get('vessel').value);
    formData.append('file', this.form.get('filename').value);

    this.uploadService.upload(formData, this.userId).subscribe(
      (res) => {
        this.uploadResponse = res;
        if( res != '')
        {
          this.uploadResponse = { status: 'response', message: "Barco "+this.form.get('vessel').value+": "+res};
          this.form.get('filename').setValue('');
          this.form.get('vessel').setValue('');
        }
      },
      (err) => this.error = err
    );
  }
}
