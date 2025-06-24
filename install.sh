#!/bin/bash

# Script de instalación para CounterStrike
# Este script:
# 1. Descarga e instala las dependencias necesarias
# 2. Compila el proyecto
# 3. Corre los tests unitarios
# 4. Instala los binarios, assets y archivos de configuración en las rutas apropiadas

set -e  # Detiene el script si algún comando falla

# Colores para mensajes
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # Sin color

# Nombre del proyecto
PROJECT_NAME="CounterStrike"

# Verificar si se está ejecutando como root 
if [ "$EUID" -ne 0 ]; then
  echo -e "${RED}Este script debe ejecutarse como root${NC}"
  echo "Por favor, ejecutar como: sudo $0"
  exit 1
fi

# Detectar el sistema operativo
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
else
    OS=$(uname -s)
fi

echo -e "${BLUE}==========================================${NC}"
echo -e "${BLUE}Instalador de $PROJECT_NAME${NC}"
echo -e "${BLUE}==========================================${NC}"

# Función para instalar dependencias según el sistema operativo
install_dependencies() {
    echo -e "${BLUE}Instalando dependencias...${NC}"
    
    if [[ "$OS" == *"Ubuntu"* ]] || [[ "$OS" == *"Debian"* ]]; then
        # Actualiza la lista de paquetes
        apt-get update
        
        # Instalar dependencias básicas de desarrollo
        apt-get install -y build-essential git cmake pkg-config wget
        
        # Instalar las dependencias de Qt
        apt-get install -y qt6-base-dev qt6-base-dev-tools qt6-multimedia-dev libqt6opengl6-dev
        
        # Instalar las bibliotecas SDL2 y extensiones
        apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
        
        # Bibliotecas de audio completas
        apt-get install -y libopus-dev libopusfile-dev libfluidsynth-dev libwavpack-dev libfreetype-dev libxmp-dev
        apt-get install -y fluidsynth wavpack libmodplug-dev
        
        # Dependencias gráficas adicionales que suelen faltar en contenedores
        apt-get install -y libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev \
            libxss-dev libwayland-dev libxkbcommon-dev libgles2-mesa-dev libgl1-mesa-dev libegl1-mesa-dev \
            libdrm-dev libgbm-dev libpulse-dev libasound2-dev 
            
        # Soporte para PipeWire (audio moderno en Linux)
        apt-get install -y libpipewire-0.3-dev
        
        # Instalar yaml-cpp explícitamente
        apt-get install -y libyaml-cpp-dev
        
    elif [[ "$OS" == *"Fedora"* ]]; then
        # Para Fedora
        dnf install -y git cmake gcc g++ make pkg-config wget
        dnf install -y qt6-qtbase-devel qt6-qttools-devel qt6-qtmultimedia-devel
        dnf install -y SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
        dnf install -y opus-devel opusfile-devel fluidsynth-devel wavpack-devel freetype-devel xmp-devel
        dnf install -y fluidsynth wavpack libmodplug-devel
        dnf install -y pipewire-devel pulseaudio-libs-devel alsa-lib-devel
        dnf install -y yaml-cpp-devel
        
    elif [[ "$OS" == *"Arch"* ]]; then
        # Para Arch
        pacman -Sy --noconfirm git cmake gcc make pkg-config wget
        pacman -Sy --noconfirm qt6-base qt6-tools qt6-multimedia
        pacman -Sy --noconfirm sdl2 sdl2_image sdl2_ttf sdl2_mixer
        pacman -Sy --noconfirm opus opusfile fluidsynth wavpack freetype2 libxmp
        pacman -Sy --noconfirm fluidsynth wavpack libmodplug
        pacman -Sy --noconfirm pipewire pulseaudio alsa-lib
        pacman -Sy --noconfirm yaml-cpp
    else
        echo -e "${RED}Sistema operativo no soportado: $OS${NC}"
        echo "Por favor, instale las dependencias manualmente:"
        echo "- git, cmake, compilador C++"
        echo "- Qt6 (Widgets, Core, Gui, Multimedia)"
        echo "- SDL2 y extensiones (image, ttf, mixer)"
        echo "- libopus, libopusfile, fluidsynth, wavpack, freetype, libxmp"
        echo "- yaml-cpp"
        exit 1
    fi
    
    echo -e "${GREEN}Dependencias instaladas correctamente${NC}"
}

# Función para compilar el proyecto
compile_project() {
    echo -e "${BLUE}Compilando el proyecto...${NC}"
    
    # Ejecutar make compile-debug desde el directorio raíz
    echo -e "${BLUE}Ejecutando make compile-debug desde directorio raíz...${NC}"
    make compile-debug
    
    # Entrar al directorio build y compilar
    echo -e "${BLUE}Entrando al directorio build y ejecutando make...${NC}"
    cd build
    
    # Compilar utilizando todos los núcleos disponibles
    if [[ "$OS" == "Darwin" ]]; then
        make -j$(sysctl -n hw.ncpu)
    else
        make -j$(nproc)
    fi
    
    echo -e "${GREEN}Proyecto compilado correctamente${NC}"
}

# Función para ejecutar tests unitarios
run_tests() {
    echo -e "${BLUE}Ejecutando tests unitarios...${NC}"
    
    # Asegurarse de que estamos en el directorio de compilación
    if [ ! -f ./taller_tests ]; then
        echo -e "${RED}No se encontró el ejecutable de tests. Asegúrate de que el proyecto se compiló correctamente.${NC}"
        return 1
    fi
    
    # Ejecutar los tests
    ./taller_tests
    
    # Verificar el resultado de los tests
    if [ $? -ne 0 ]; then
        echo -e "${RED}Los tests han fallado. No se puede continuar con la instalación.${NC}"
        return 1
    fi
    
    echo -e "${GREEN}Todos los tests han pasado correctamente${NC}"
    return 0
}

# Función para instalar el proyecto
install_project() {
    echo -e "${BLUE}Instalando $PROJECT_NAME...${NC}"

    # Crear directorios necesarios
    mkdir -p /usr/bin
    mkdir -p /var/$PROJECT_NAME/assets
    mkdir -p /etc/$PROJECT_NAME/maps

    # Copiar binarios
    cp build/client/taller_client /usr/bin/$PROJECT_NAME-client
    cp build/server/taller_server /usr/bin/$PROJECT_NAME-server
    cp build/editor/taller_editor /usr/bin/$PROJECT_NAME-editor

    # Copiar carpeta de assets
    cp -r client/assets/* /var/$PROJECT_NAME/assets/
    
    # Copiar mapas
    cp -r server/maps/* /etc/$PROJECT_NAME/maps/

    # Establecer permisos ejecutables
    chmod +x /usr/bin/$PROJECT_NAME-client
    chmod +x /usr/bin/$PROJECT_NAME-server
    chmod +x /usr/bin/$PROJECT_NAME-editor

    # Crear enlaces simbólicos para que la aplicación encuentre los recursos
    mkdir -p /usr/bin/client
    ln -sf /var/$PROJECT_NAME/assets /usr/bin/client/assets
    
    echo -e "${GREEN}Instalación completada correctamente${NC}"
    
    # Mostrar información de uso
    echo -e "${BLUE}=== Información de uso ===${NC}"
    echo -e "Para ejecutar el cliente: ${GREEN}$PROJECT_NAME-client${NC}"
    echo -e "Para ejecutar el servidor: ${GREEN}$PROJECT_NAME-server${NC}"
    echo -e "Para ejecutar el editor de mapas: ${GREEN}$PROJECT_NAME-editor${NC}"
    echo -e "${BLUE}=========================${NC}"
    
    # Detectar si estamos en Docker
    if grep -q docker /proc/1/cgroup 2>/dev/null || [ -f /.dockerenv ]; then
        echo -e "${BLUE}=== Ejecución en Docker ===${NC}"
        echo -e "Se ha detectado que este instalador se está ejecutando en un contenedor Docker."
        echo -e "Para aplicaciones gráficas, use el script auxiliar: ${GREEN}run-vnc.sh${NC}"
        echo -e "Ejemplo: ${GREEN}run-vnc.sh CounterStrike-editor${NC}"
        echo -e "Luego conecte desde su host a ${GREEN}vnc://localhost:5900${NC}"
        echo -e "${BLUE}=========================${NC}"
    fi
}

# Función principal
main() {
    # 1. Instalar dependencias
    install_dependencies
    
    # 2. Compilar el proyecto
    compile_project
    
    # 3. Ejecutar tests
    if ! run_tests; then
        exit 1
    fi
    
    # 4. Instalar el proyecto
    install_project
    
    echo -e "${GREEN}¡$PROJECT_NAME ha sido instalado correctamente!${NC}"
}

# Ejecutar la función principal
main

# Instrucciones para Docker
cat << "EOT"

==============================================
NOTA PARA ENTORNOS DOCKER
==============================================

Si estás ejecutando este instalador en un contenedor Docker y deseas
usar las aplicaciones gráficas (cliente/editor), necesitarás configurar
el acceso al servidor X11.

Para ejecutar CounterStrike en Docker con interfaz gráfica:

1. En tu host (fuera de Docker), permite conexiones X11:
   $ xhost +local:docker

2. Inicia el contenedor con las opciones adecuadas:
   $ docker run -it --name counterstrike \
     -e DISPLAY=$DISPLAY \
     -v /tmp/.X11-unix:/tmp/.X11-unix \
     --network=host \
     ubuntu:latest bash

3. Dentro del contenedor, instala el proyecto como de costumbre:
   $ apt update && apt install -y git sudo
   $ git clone https://github.com/Sebakrag/taller-CounterStrike.git
   $ cd taller-CounterStrike
   $ chmod +x install.sh
   $ sudo ./install.sh

Si solo necesitas probar el servidor sin interfaz gráfica, puedes
ejecutarlo directamente en cualquier contenedor Docker sin 
configuración adicional:
   $ CounterStrike-server

==============================================
EOT
