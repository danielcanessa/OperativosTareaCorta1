#!/bin/bash
echo "Escribiendo hora que se paro el servicio en el archivo";
echo "Hora en que se paro el servicio: " >> ArchivoInformacionStopServicio
timedatectl >> ArchivoInformacionStopServicio
echo "Listo";


