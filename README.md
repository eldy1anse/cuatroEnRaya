# cuatroEnRaya


Este proyecto implementa el juego de Cuatro en Raya mediante el modelo Cliente/Servidor. 

Se utilizan sockets TCP/IP para permitir la comunicación entre un cliente y un servidor. El servidor maneja múltiples conexiones concurrentes, lo que permite a varios clientes jugar simultáneamente.

Para compilar se debe ingresar tanto a la carpeta de cliente como la del servidor y ejecutar el comando "make".

* Para ejecutar el servidor, se debe utilizar el comando ./servidor <puerto>
* Para ejecutar el cliente, se debe utilizar el comando ./cliente <dirección_ip_del_servidor> <puerto>

Por ejemplo, puede ejecutar el comando ./servidor 8080 en la carpeta servidor y luego ejecutar ./cliente <dirección_ip_del_servidor> 8080 en la carpeta cliente.

