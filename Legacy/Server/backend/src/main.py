#!/usr/bin/python
# Para ejecutar: flask run -h 0.0.0.0
 
# coding=utf-8
from flask import Flask, jsonify, request, render_template, send_from_directory
from flask_cors import CORS, cross_origin
import os
import uuid
import loadcsv

# creating the Flask application
app = Flask(__name__, static_folder='frontend', static_url_path='', template_folder='frontend' )
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

APP_ROOT = os.path.dirname(os.path.abspath(__file__))

@app.route('/')
@cross_origin()
def home():
    return render_template('index.html')

		
@app.route('/hola')
@cross_origin()
def get_hola():
    return jsonify('Hola Mundo')

@app.route("/upload", methods=["POST"])
@cross_origin()
def upload():
    response = ""
	# make a UUID based on the host ID and current time
    newguid = str(uuid.uuid1())
    print("UUID:",newguid)
    vessel = request.form.get('vessel')
    if not vessel:
        return jsonify('Es necesario especificar un nombre de barco...'),400
    print("Barco:",vessel)
    target = os.path.join(APP_ROOT, 'files')
    print(target)
    
    if not os.path.isdir(target):
        os.mkdir(target)
    print(request.files.getlist("file"))
    for upload in request.files.getlist("file"):
        print(upload)
        print("{} is the file name".format(upload.filename))
        filename = newguid+"-"+upload.filename
        # This is to verify files are supported
        print('antes')
        ext = os.path.splitext(filename)[1]
        print('despues')
        if (ext == ".csv"):
            print("File supported moving on...")
        else:
            return jsonify('Files uploaded are not supported...'),400
        destination = "/".join([target, filename])
        print("Accept incoming file:", filename)
        print("Save it to:", destination)
        upload.save(destination)
        response = loadcsv.load(request.form.get('vessel'),destination)

    return jsonify(response + "\nFin de la carga del archivo.")