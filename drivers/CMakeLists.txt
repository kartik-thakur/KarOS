target_sources(KarOS PUBLIC
	drivers-init.c
)

add_subdirectory(i2c)
add_subdirectory(thermal)
