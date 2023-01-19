# Proyecto Final

## Movimiento del jugador:

El jugador se mueve con las teclas WASD. La cámara se mueve con el ratón.

## Armas:

Con el click izquierdo (LMB) se dispara el arma actual. Con las teclas '1', '2', '3', '4' y '5' se cambia entre las armas disponibles.

Si se lanza un cohete justo debajo del jugador (4), el jugador sale disparado hacia arriba.

## Generadores de fuerzas antiguos

Ya que no causaban problemas, no he quitado los generadores de fuerzas. Estos generadores no afectan ni al jugador ni a los enemigos.

Los generadores pueden coexistir y se activan y desactivan según

Con la tecla 'U' se crea un generador de gravedad inversa, que tiene una aceleración de 100 en el eje Y.

Con la tecla 'I' se crea un generador de viento, que tiene velocidad en el eje negativo de X y como constates K1 = 1, K2 = 0, es un viento sin turbulencias.

Con la tecla 'O' se crea un generador de torbellino, que se encuentra en el (0, 0, 0) y tiene como constantes K = 1, K1 = 1 y K2 = 0. En torbellino hereda de WindGenerator.

Con la tecla 'P' se crea una explosión en el (0, 0, 0) con rango inicial 50. Esta explosion tiene K = 100000, que hace que sea una fuerza repentina.

Con la tecla 'Y' se desactivan todas las fuerzas.

Además, con la tecla 'M' se borran todas las particulas en la escena.

## Generador de particulas rigidas

De forma similar al apartado anterior, ya que no causaba problemas se puede utilizar el generador de particulas rígidas con la tecla '9'.

Con la tecla 'Q' se desactiva su generación.