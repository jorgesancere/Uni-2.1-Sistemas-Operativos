#!/usr/bin/env bash
# Ejercicio 1: monitor.sh

# $1 es el primer parametro (-n, -u, -p, -e, -h)
# $2 es el segundo parametro (depende del primer parametro lo uso pa una cosa u otra)


#----------------------------------------------------

# 1. Muestre el top N procesos con mayor consumo de memoria (-n 5)

# Aqui lo que hago es mostrar los procesos (ps aux), ordenados por memoria (sort mem) y que unicamente se muestren las x + 1 primeras lineas (la linea del head que es la descripcion de cada columna + x procesos (en el ejercicio seria  5+ 1

if [ "$1" = "-n" ]; then
	ps ax -o user,pid,%cpu,%mem,comm --sort=-%mem | head -n 6

# 2. Muestre solo los procesos que pertenecen al usuario especificado (-u root)

# Ahora cuando diga -u entonces hago lo de el 1 + mostrar unicamente lo del usuario que me digan (el usuario es $2) gracias al -u root (uso lo del apartado 1 porque de eso va el ejercicio, de monitorizar los procesos que consumen mucha RAM)
elif [ "$1" = "-u" ]; then
    ps -u "$2" aax -o user,pid,%cpu,%mem,comm --sort=-%mem | head -n 6

# 3. Muestre solo los procesos cuyo comando contenga el patrón indicado. Útil para localizar entrenamientos o servicios concretos (ej. python, torchrun, jupyter) (-p bash)
# Ahora uso el comando -p bash para solo mostrar lo que yo quiero mostrar (busco con el grep una app o serivio o lo que sea) + mostrarlos ordenados por consumo de RAM (apartado 1)
elif [ "$1" = "-p" ]; then
    ps ax -o user,pid,%cpu,%mem,comm --sort=-%mem | grep "$2" | head -n 6

# 4. Guarde los errores del log del sistema en un fichero dado ( -e error.txt)
# Con el comando -e error.txt ahora lo que hago es buscar error (el -i es para buscar ignorando mayusculas o minusculas)
elif [ "$1" = "-e" ]; then
    grep -i "error" /var/log/syslog > "$2"
    echo "Errores guardados en $2"

# 5. Imprima la ayuda del script en caso de que se solicite (-h)
# imprime un mensajito guay de ayuda
elif [ "$1" = "-h" ]; then
    echo "Uso: ./monitor.sh [-n N] [-u usuario] [-p patron] [-e fichero] [-h]"

# Le meto al else una salida de por si acaso, un echo con la ayuda x si no sabes usar esto
else
	echo "Uso: ./monitor.sh [-n N] [-u usuario] [-p patron] [-e fichero] [-h]"
fi
#----------------------------------------------------



#Con el ps aux me salia mucha cosa y no se entendia, pero despues de buscar, si quito el aux y meto el -o puedo mostrar lolo lo que yo quiera
