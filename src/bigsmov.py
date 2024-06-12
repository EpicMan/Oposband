import os
exts = ['.c','.h','.txt','.prf'];
for r, ds, fs in os.walk("."):
 for fn in fs:
  if (fn != fn.lower()):
   if (os.path.splitext(fn)[1]).lower() in exts:
    os.rename(os.path.join(r, fn), os.path.join(r, fn.lower()));
