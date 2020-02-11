import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormGroup } from '@angular/forms';
import { HttpClient } from '@angular/common/http';


@Component({
  selector: 'app-formdata-upload',
  templateUrl: './formdata-upload.component.html',
  styleUrls: ['./formdata-upload.component.css']
})
export class FormdataUploadComponent implements OnInit {

  SERVER_URL: string = "http://localhost:5000";

  constructor(private formBuilder: FormBuilder, private http: HttpClient) { }

  form: FormGroup;
  error: string;
  userId: number = 1;
  uploadResponse: string = '';

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
    this.uploadResponse = '';
    const formData = new FormData();

    formData.append('vessel', this.form.get('vessel').value);
    formData.append('file', this.form.get('filename').value);

    let uploadURL = `${this.SERVER_URL}/upload`;
    this.http.post(uploadURL, formData).subscribe(
      (res) => {
        this.uploadResponse = "Barco " + this.form.get('vessel').value + ": \n";
        this.uploadResponse += "----------------\n";
        this.uploadResponse += res;
        this.form.get('filename').setValue('');
        this.form.get('vessel').setValue('');
      },
      (err) => this.error = err
    );
  }
}
