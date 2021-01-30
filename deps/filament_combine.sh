ar -M <<EOM
  CREATE libfilament-full.a
  ADDLIB filament/lib/x86_64/libbackend.a
  ADDLIB filament/lib/x86_64/libbluegl.a
  ADDLIB filament/lib/x86_64/libbluevk.a
  ADDLIB filament/lib/x86_64/libcamutils.a
  ADDLIB filament/lib/x86_64/libdracodec.a
  ADDLIB filament/lib/x86_64/libfilabridge.a
  ADDLIB filament/lib/x86_64/libfilaflat.a
  ADDLIB filament/lib/x86_64/libfilamat.a
  ADDLIB filament/lib/x86_64/libfilamat_lite.a
  ADDLIB filament/lib/x86_64/libfilament.a
  ADDLIB filament/lib/x86_64/libfilameshio.a
  ADDLIB filament/lib/x86_64/libgeometry.a
  ADDLIB filament/lib/x86_64/libgltfio.a
  ADDLIB filament/lib/x86_64/libgltfio_core.a
  ADDLIB filament/lib/x86_64/libibl.a
  ADDLIB filament/lib/x86_64/libimage.a
  ADDLIB filament/lib/x86_64/libmatdbg.a
  ADDLIB filament/lib/x86_64/libmeshoptimizer.a
  ADDLIB filament/lib/x86_64/libshaders.a
  ADDLIB filament/lib/x86_64/libsmol-v.a
  ADDLIB filament/lib/x86_64/libutils.a
  ADDLIB filament/lib/x86_64/libviewer.a
  SAVE
  END
EOM