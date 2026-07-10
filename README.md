# Clone Hero Com ARDUINO

<img width="640" height="360" alt="image" src="https://github.com/user-attachments/assets/b966f8a4-64e7-45b3-b93b-039ac5113ecb" />


Projeto simples para usar guitarra ou bateria feita com ESP32/ARDUINO no jogo Clone Hero. 

https://clonehero.net/

Inicialmente usamos a abordagem de conectar os instrumentos no ESP32 e mandar pela wifi usando udp,
Depois, percebemos que os delays estavam bem ruins e atrapalhando bastante,
passamos a usar uma placa arduino lendo botões, push buttons do grande, e escrevendo na serial
o programa em python monitora a porta serial e clica nos botões do controle virtual usando a
lib vigebus

## O que precisa
- Emulador de controle vigembus instalado no windows e dps reiniciar o pc (https://vigembus.com/ )
- Arduino IDE
- Python 3.11 instalado no PC
- Clone Hero instalado (Jogo)

## Instalar dependencias do Python

No terminal, dentro desta pasta:

```bash
 pip install vgamepad
```

Nesta pasta, execute:

```bash
python hero.py
```

## Mapeamento da guitarra

```text
GREEN  -> A
RED    -> S
YELLOW -> J
BLUE   -> K
ORANGE -> L
STRUM  -> STRUM DOWN SETA PRA BAIXO
START  -> START
```

## Mapeamento da bateria

```text
DRUM_RED    -> S
DRUM_YELLOW -> J
DRUM_BLUE   -> K
DRUM_GREEN  -> A
DRUM_ORANGE -> BUMBO
KICK        -> STRUM DONW (SETA PRA BAIXO)
START       -> START
```


Lista de peças:

https://www.robocore.net/protoboard/protoboard-400-pontos

https://www.robocore.net/botao-chave/kit-push-button-com-capas-coloridas-x25-unidades

https://www.robocore.net/ferramentas/alicate-de-corte-de-precisao

https://www.robocore.net/ferramentas/fita-isolante-3m-preta

https://www.robocore.net/placa-arduino/placa-uno-r3

https://www.robocore.net/cabo/cabo-usb-ab-30cm

https://www.robocore.net/cabo/jumpers-macho-macho-x40-cabos

Opcional:

https://www.robocore.net/ferramentas/kit-solda-110v

CONECTOR DE 5 VIAS (Para os padas)

Conector de 2 VIAS (Para o bumbo)

COLA QUENTE

BASTÃO DE COLA QUENTE

FITA ADESIVA

ESPUMAS

CAPS DE CANOS DE PVC

CABO CAT5 (CABO DE INTERNET) 3 METROS


Compre na robocore.net use o cupom: LCSISTEMAS
