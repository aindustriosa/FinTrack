import { HttpClient, HttpEvent, HttpErrorResponse, HttpEventType } from  '@angular/common/http';
import { map } from  'rxjs/operators';
import { Injectable } from '@angular/core';
import { stringify } from 'querystring';

@Injectable({
  providedIn: 'root'
})
export class UploadService {

  SERVER_URL: string = "http://localhost:5000";
  constructor(private httpClient: HttpClient) { }

  public upload(data, userId) {
    let uploadURL = `${this.SERVER_URL}/upload`;

    return this.httpClient.post<any>(uploadURL, data, {
      reportProgress: true,
      observe: 'events'
    }).pipe(map((event) => {

      switch (event.type) {

        case HttpEventType.UploadProgress:
          const progress = Math.round(100 * event.loaded / event.total);
          return { status: 'progress', message: progress };

          case HttpEventType.Response:
              return event.body;
        default:
          //return `Unhandled event: ${event.type}`;
          return '';
      }
    })
    );
  }
}

