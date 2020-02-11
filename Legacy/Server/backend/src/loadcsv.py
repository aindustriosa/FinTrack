import csv
import random
from datetime import datetime

import MySQLdb
import sys

def load(vessel,filename):
    result=""

    mydb = MySQLdb.connect(host='localhost',
        user='fintrackuser',
        passwd='<password>',
        db='fintrackdb')
    cursor = mydb.cursor()

    if not filename:
        return "Es necesario pasar como primer parametro el archivo a importar\n";

    if not vessel:
        return "Es necesario pasar como segundo parametro el nombre del barco\n";

    csv_data = csv.reader(open(filename))

    name = vessel

    firstline = True
    i = 0
    for row in csv_data:
        if firstline:  # skip first line
            firstline = False
            continue

        try:
            voltaje = (int(row[14]) * 0.021402322) + 2.179756098
            corriente = (int(row[16]) * 0.048407688) - 24.73626053
            
            cursor.execute('INSERT INTO data_SI (identifier,name,TimeStamp_ms,Fecha,Latitud,Longitud,Altura,Latitud_Err,'
                        'Longitud_Err,Altura_Err,Voltaje,Voltaje_std,Corriente,Corriente_std,Luz,Luz_std,Presion,'
                        'Presion_std,Potencia,Aceleracion_X,Aceleracion_X_std,Aceleracion_Y,Aceleracion_Y_std,'
                        'Aceleracion_Z,Aceleracion_Z_std,Vel_Angular_X,Vel_Angular_X_std,Vel_Angular_Y,Vel_Angular_Y_std,'
                        'Vel_Angular_Z,Vel_Angular_Z_std,Campo_Magnetico_X,Campo_Magnetico_X_std,Campo_Magnetico_Y,'
                        'Campo_Magnetico_Y_std,Campo_Magnetico_Z,Campo_Magnetico_Z_std) VALUES (%(identifier)s, %(name)s, '
                        '%(TimeStamp_ms)s, %(Fecha)s, %(Latitud)s, %(Longitud)s, %(Altura)s, %(Latitud_Err)s, '
                        '%(Longitud_Err)s, %(Altura_Err)s, %(Voltaje)s, %(Voltaje_std)s, %(Corriente)s, %(Corriente_std)s, '
                        '%(Luz)s, %(Luz_std)s, %(Presion)s, %(Presion_std)s, %(Potencia)s, %(Aceleracion_X)s, '
                        '%(Aceleracion_X_std)s, %(Aceleracion_Y)s, %(Aceleracion_Y_std)s, %(Aceleracion_Z)s, '
                        '%(Aceleracion_Z_std)s, %(Vel_Angular_X)s, %(Vel_Angular_X_std)s, %(Vel_Angular_Y)s, '
                        '%(Vel_Angular_Y_std)s, %(Vel_Angular_Z)s, %(Vel_Angular_Z_std)s, %(Campo_Magnetico_X)s, '
                        '%(Campo_Magnetico_X_std)s, %(Campo_Magnetico_Y)s, %(Campo_Magnetico_Y_std)s, '
                        '%(Campo_Magnetico_Z)s, %(Campo_Magnetico_Z_std)s)',
                        {'identifier': row[0],
                            'name': name,
                            'TimeStamp_ms': row[1],
                            'Fecha': datetime(int(row[2]), int(row[3]), int(row[4]), int(row[5]), int(row[6]), int(row[7])),
                            'Latitud': round(float(row[8])/10000000,6),
                            'Longitud': round(float(row[9])/10000000,6),
                            'Altura': row[10],
                            'Latitud_Err': int(row[11])/100,
                            'Longitud_Err': int(row[12])/100,
                            'Altura_Err': int(row[13])/100,
                            'Voltaje': voltaje,
                            'Voltaje_std': row[15],
                            'Corriente': corriente,
                            'Corriente_std': row[17],
                            'Luz': 1147988.19 * pow(int(row[18]), -1.111111111),
                            'Luz_std': row[19],
                            'Presion': row[20],
                            'Presion_std': row[21],
                            'Potencia': voltaje * corriente,
                            'Aceleracion_X': (int(row[22]) * 0.000244141),
                            'Aceleracion_X_std': row[23],
                            'Aceleracion_Y': (int(row[24]) * 0.000244141),
                            'Aceleracion_Y_std': row[25],
                            'Aceleracion_Z': (int(row[26]) * 0.000244141),
                            'Aceleracion_Z_std': row[27],
                            'Vel_Angular_X': (int(row[28]) * 0.007633588) + 0.00000000000000203012,
                            'Vel_Angular_X_std': row[29],
                            'Vel_Angular_Y': (int(row[30]) * 0.007633588) + 0.00000000000000203012,
                            'Vel_Angular_Y_std': row[31],
                            'Vel_Angular_Z': (int(row[32]) * 0.007633588) + 0.00000000000000203012,
                            'Vel_Angular_Z_std': row[33],
                            'Campo_Magnetico_X': (int(row[34]) * 0.000585699) + 0.074969475,
                            'Campo_Magnetico_X_std': row[35],
                            'Campo_Magnetico_Y': (int(row[36]) * 0.000585699) + 0.074969475,
                            'Campo_Magnetico_Y_std': row[37],
                            'Campo_Magnetico_Z': (int(row[38]) * 0.000585699) + 0.074969475,
                            'Campo_Magnetico_Z_std': row[39]})
        except ValueError:
            result+="Error al procesar los datos de la fila "+str(i)+"\n"
        i += 1

    mydb.commit()
    cursor.close()
    result+="Done\n"
    return result
