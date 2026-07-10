import serial
import socket
import threading
import time
import vgamepad as vg

# ======================================================
# CONFIGURAÇÕES
# ======================================================

SERIAL_PORT = "COM4"
SERIAL_BAUD = 115200

UDP_IP = "0.0.0.0"
UDP_PORT_GUITARRA = 5000

BUFFER_SIZE = 1024

PULSE = 0.004          # duração do pulso da bateria (4 ms)

LOG_ATIVO = True       # True = mostra eventos | False = silencioso

# ======================================================
# CONTROLES VIRTUAIS
# ======================================================

guitarra = vg.VX360Gamepad()
bateria = vg.VX360Gamepad()

print("Controle 1 -> Guitarra")
print("Controle 2 -> Bateria")

# ======================================================
# MAPA GUITARRA
# ======================================================

MAPA_GUITARRA = {
    "GREEN": vg.XUSB_BUTTON.XUSB_GAMEPAD_A,
    "RED": vg.XUSB_BUTTON.XUSB_GAMEPAD_B,
    "YELLOW": vg.XUSB_BUTTON.XUSB_GAMEPAD_Y,
    "BLUE": vg.XUSB_BUTTON.XUSB_GAMEPAD_X,
    "ORANGE": vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER,
    "START": vg.XUSB_BUTTON.XUSB_GAMEPAD_START,
}

# ======================================================
# MAPA BATERIA
# ======================================================

MAPA_BATERIA = {
    "DRUM_RED": vg.XUSB_BUTTON.XUSB_GAMEPAD_B,
    "DRUM_YELLOW": vg.XUSB_BUTTON.XUSB_GAMEPAD_Y,
    "DRUM_BLUE": vg.XUSB_BUTTON.XUSB_GAMEPAD_X,
    "DRUM_GREEN": vg.XUSB_BUTTON.XUSB_GAMEPAD_A,
    "DRUM_ORANGE": vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER,
    "KICK": vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER,
    "START": vg.XUSB_BUTTON.XUSB_GAMEPAD_START,
}

# ======================================================
# SERIAL
# ======================================================

ser = serial.Serial(
    SERIAL_PORT,
    SERIAL_BAUD,
    timeout=0.001
)

# ======================================================
# AUXILIARES
# ======================================================

def pulse_button(gamepad, button):
    gamepad.press_button(button=button)
    gamepad.update()

    time.sleep(PULSE)

    gamepad.release_button(button=button)
    gamepad.update()


def strum_down():
    guitarra.directional_pad(
        direction=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN
    )
    guitarra.update()


def strum_up():
    guitarra.directional_pad(direction=0)
    guitarra.update()

# ======================================================
# THREAD SERIAL - BATERIA
# ======================================================

def bateria_serial():

    print("Serial bateria iniciada")

    while True:

        try:

            linha = ser.readline().decode(
                "utf-8",
                errors="ignore"
            ).strip()

            if not linha:
                continue

            try:
                botao, acao = linha.split(":")
            except ValueError:
                continue

            botao = botao.upper()
            acao = acao.upper()

            if acao != "DOWN":
                continue

            if botao not in MAPA_BATERIA:
                continue

            if LOG_ATIVO:
                print(f"[DRUM] {botao}")

            pulse_button(
                bateria,
                MAPA_BATERIA[botao]
            )

        except Exception as erro:
            print("SERIAL:", erro)

# ======================================================
# THREAD UDP - GUITARRA
# ======================================================

def guitarra_udp():

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT_GUITARRA))

    print(f"UDP guitarra porta {UDP_PORT_GUITARRA}")

    while True:

        try:

            dados, endereco = sock.recvfrom(BUFFER_SIZE)

            mensagem = dados.decode(
                "utf-8",
                errors="ignore"
            ).strip()

            try:
                botao, acao = mensagem.split(":")
            except ValueError:
                continue

            botao = botao.upper()
            acao = acao.upper()

            if botao == "STRUM":

                if LOG_ATIVO:
                    print(f"[GUITAR] STRUM:{acao}")

                if acao == "DOWN":
                    strum_down()
                else:
                    strum_up()

                continue

            if botao not in MAPA_GUITARRA:
                continue

            if LOG_ATIVO and acao == "DOWN":
                print(f"[GUITAR] {botao}")

            if acao == "DOWN":
                guitarra.press_button(
                    button=MAPA_GUITARRA[botao]
                )

            elif acao == "UP":
                guitarra.release_button(
                    button=MAPA_GUITARRA[botao]
                )

            guitarra.update()

        except Exception as erro:
            print("UDP:", erro)

# ======================================================
# THREADS
# ======================================================

threading.Thread(
    target=bateria_serial,
    daemon=True
).start()

threading.Thread(
    target=guitarra_udp,
    daemon=True
).start()

# ======================================================
# INICIALIZAÇÃO
# ======================================================

print("")
print("======================================")
print(" Clone Hero Bridge")
print("======================================")
print(f"Serial   : {SERIAL_PORT} @ {SERIAL_BAUD}")
print(f"UDP       : {UDP_PORT_GUITARRA}")
print(f"Logs      : {'ATIVADOS' if LOG_ATIVO else 'DESATIVADOS'}")
print("")
print("Controle 1 -> Guitarra (UDP)")
print("Controle 2 -> Bateria (Serial)")
print("")
print("Pronto!")
print("")

# ======================================================
# LOOP PRINCIPAL
# ======================================================

try:

    while True:
        time.sleep(1)

except KeyboardInterrupt:

    print("\nEncerrando...")

    for b in MAPA_GUITARRA.values():
        guitarra.release_button(button=b)

    guitarra.directional_pad(direction=0)
    guitarra.update()

    for b in MAPA_BATERIA.values():
        bateria.release_button(button=b)

    bateria.update()

    ser.close()

    print("Finalizado.")
