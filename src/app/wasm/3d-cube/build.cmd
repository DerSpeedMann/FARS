emcc -Os ./soil/libSOIL.bc ./3d-cube.c -o ../../../assets/wasm/3d-cube.js -s LEGACY_GL_EMULATION=1 -I ./soil -s EXPORTED_RUNTIME_METHODS="[\'ccall\']" -s FORCE_FILESYSTEM=1 -s MODULARIZE=1 -s EXPORT_NAME=Cube3dModule