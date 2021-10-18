target_sources(KarOS PUBLIC
	drivers-init.c
)

add_subdirectory(display)
add_subdirectory(i2c)
add_subdirectory(thermal)
