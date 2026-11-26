#!/bin/bash
# setup_sii.sh — deja el escritorio virtual listo para las prácticas de SII
# Rellena SOLO el EMAIL una vez:
NOMBRE="Karina Díaz Escalante"
EMAIL="dandiesc9@gmail.com"
MI_REPO="https://github.com/kariinadiiaz/sii-57417.git"
REPO_PROFE="https://github.com/raquelcedazo/sistemas-informaticos-industriales.git"
CARPETA="$HOME/sii"

echo "== 1. Paquetes =="
FALTAN=""
for p in g++ git cmake make unzip; do
  command -v "$p" >/dev/null 2>&1 || FALTAN="$FALTAN $p"
done
for p in freeglut3-dev libglu1-mesa-dev mesa-common-dev; do
  dpkg -s "$p" >/dev/null 2>&1 || FALTAN="$FALTAN $p"
done

if [ -n "$FALTAN" ]; then
  echo "Instalando:$FALTAN"
  sudo apt-get update -qq && sudo apt-get install -y $FALTAN \
    || echo "!! No se pudo instalar (¿sin sudo?). Faltan:$FALTAN"
else
  echo "Todo instalado ya."
fi

echo "== 2. Git =="
git config --global user.name  "$NOMBRE"
git config --global user.email "$EMAIL"
git config --global init.defaultBranch main
git config --global pull.rebase false
# Guarda el token en memoria 8 h: solo lo pegas la primera vez que hagas push
git config --global credential.helper 'cache --timeout=28800'

echo "== 3. Repo =="
if [ -d "$CARPETA/.git" ]; then
  git -C "$CARPETA" pull
else
  git clone "$MI_REPO" "$CARPETA"
fi

if [ -n "$REPO_PROFE" ]; then
  git -C "$CARPETA" remote get-url profe >/dev/null 2>&1 \
    || git -C "$CARPETA" remote add profe "$REPO_PROFE"
  git -C "$CARPETA" fetch profe
fi

echo
echo "LISTO. Ahora escribe:  cd $CARPETA"
echo "Al terminar la sesion:  cd && rm -rf $CARPETA ~/.gitconfig"
