#!/bin/bash

# Colores para mensajes
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Verificar si se proporcionó un parámetro
if [ $# -ne 1 ]; then
    echo -e "${RED}Error: Debe proporcionar el número de clientes como parámetro${NC}"
    echo "Uso: $0 <número_de_clientes>"
    exit 1
fi

# Obtener el número de clientes
NUM_CLIENTS=$1

# Verificar que sea un número positivo
if ! [[ "$NUM_CLIENTS" =~ ^[0-9]+$ ]] || [ "$NUM_CLIENTS" -lt 1 ]; then
    echo -e "${RED}Error: El número de clientes debe ser un entero positivo${NC}"
    exit 1
fi

# Verificar que los binarios existan
if [ ! -f "./build/taller_server" ] || [ ! -f "./build/taller_client" ]; then
    echo -e "${BLUE}Compilando el proyecto...${NC}"
    make compile-debug
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error: La compilación falló${NC}"
        exit 1
    fi
fi

# Puerto para el servidor
PORT=8080

# Iniciar el servidor en segundo plano
echo -e "${GREEN}Iniciando servidor en puerto $PORT...${NC}"
./build/taller_server $PORT > server_log.txt 2>&1 &
SERVER_PID=$!

# Esperar un momento para que el servidor se inicie
sleep 2

# Verificar si el servidor está ejecutándose
if ! ps -p $SERVER_PID > /dev/null; then
    echo -e "${RED}Error: El servidor no pudo iniciarse${NC}"
    cat server_log.txt
    exit 1
fi

echo -e "${GREEN}Servidor iniciado con PID $SERVER_PID${NC}"

# Iniciar los clientes
echo -e "${BLUE}Iniciando $NUM_CLIENTS clientes...${NC}"
CLIENT_PIDS=()

for ((i=1; i<=$NUM_CLIENTS; i++)); do
    echo -e "${BLUE}Iniciando cliente $i...${NC}"
    ./build/taller_client > client_${i}_log.txt 2>&1 &
    CLIENT_PID=$!
    CLIENT_PIDS+=($CLIENT_PID)
    echo -e "${GREEN}Cliente $i iniciado con PID $CLIENT_PID${NC}"
    sleep 0.5
done

echo -e "${GREEN}Todos los procesos iniciados correctamente${NC}"
echo "Servidor PID: $SERVER_PID"
echo "Clientes PIDs: ${CLIENT_PIDS[@]}"
echo ""
echo -e "${BLUE}Para detener todos los procesos, presione Ctrl+C${NC}"

# Función para manejar la señal de interrupción
cleanup() {
    echo -e "\n${BLUE}Deteniendo todos los procesos...${NC}"
    
    # Detener clientes
    for pid in "${CLIENT_PIDS[@]}"; do
        if ps -p $pid > /dev/null; then
            echo "Deteniendo cliente con PID $pid"
            kill $pid
        fi
    done
    
    # Detener servidor
    if ps -p $SERVER_PID > /dev/null; then
        echo "Deteniendo servidor con PID $SERVER_PID"
        kill $SERVER_PID
    fi
    
    echo -e "${GREEN}Todos los procesos detenidos${NC}"
    exit 0
}

# Registrar la función de limpieza para la señal SIGINT (Ctrl+C)
trap cleanup SIGINT

# Esperar a que el usuario presione Ctrl+C
while true; do
    sleep 1
done
