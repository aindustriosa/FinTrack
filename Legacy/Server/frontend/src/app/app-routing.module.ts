import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { FormdataUploadComponent } from './formdata-upload/formdata-upload.component';

const routes: Routes = [
  {path: '', component: FormdataUploadComponent}
];
@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
