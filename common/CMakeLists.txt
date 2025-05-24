target_sources(taller_common
    PRIVATE
    # .cpp files
        socket.cpp
        resolver.cpp
        liberror.cpp
        resolvererror.cpp

    PUBLIC
    # .h files
        socket.h
        resolver.h
        liberror.h
        resolvererror.h
        threads.h
    )
