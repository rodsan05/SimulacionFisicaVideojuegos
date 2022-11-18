# Práctica 3
He implementado 4 sistemas de fuerzas distintos para las particulas, uno de gravedad, uno de viento, uno de torbellino y uno de explosiones. Se reutilizan los sistemas de particulas de la practica anterior y además se añade uno nuevo que se crea con la tecla '5', que crea particulas de diversas masas y volúmenes, a mayor masa mayor volumen.

Los generadores pueden coexistir y se activan y desactivan según 

- Con la tecla 'U' se crea un generador de gravedad, que tiene una aceleración de -10 en el eje Y.

- Con la tecla 'I' se crea un generador de viento, que tiene velocidad en el eje negativo de X y como constates K1 = 1, K2 = 0, es un viento sin turbulencias.

- Con la tecla 'O' se crea un generador de torbellino, que se encuentra en el (0, 0, 0) y tiene como constantes K = 1, K1 = 1 y K2 = 0. En torbellino hereda de WindGenerator.

- Con la tecla 'P' se crea una explosión en el (0, 0, 0) con rango inicial 50. Esta explosion tiene K = 100000, que hace que sea una fuerza repentina.

- Con la tecla 'Y' se desactivan todas las fuerzas.

Además, con la tecla 'M' se borran todas las particulas en la escena.

Por último, respecto a la implementación, he cambiado el ForceRegistry a un multimap<Particle*, ForceGenerator*> que permite el mismo comportamiento pero hace que la complejidad de la operación de borrar una particula del registro pase de ser O(N) a O(logN + nKeysPart). Esto hace que no ocurra lag al borrar un alto número de partículas del registro. 