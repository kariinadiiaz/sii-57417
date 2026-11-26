CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT (TENIS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

SET(COMMON_SRCS
    src/Esfera.cpp
    src/Plano.cpp
    src/Raqueta.cpp
    src/Vector2D.cpp
)

# Servidor (Práctica 4): física del juego, logger y hilo de teclas
ADD_EXECUTABLE(servidor src/servidor.cpp src/MundoServidor.cpp ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(servidor glut GL GLU pthread)

# Cliente (Práctica 4): dibuja, manda teclas y habla con el bot
ADD_EXECUTABLE(cliente src/cliente.cpp src/MundoCliente.cpp ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(cliente glut GL GLU)

# Logger (Práctica 2)
ADD_EXECUTABLE(logger src/logger.cpp)

# Bot (Práctica 3)
ADD_EXECUTABLE(bot src/bot.cpp ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(bot glut GL GLU)
