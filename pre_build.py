import os
import shutil

def copy_user_setup(*args, **kwargs):
    # Путь к нашему User_Setup.h
    src = os.path.join(env["PROJECT_DIR"], "lib", "TFT_eSPI", "User_Setup.h")
    
    # Путь к библиотеке TFT_eSPI
    dst_dir = os.path.join(env["PROJECT_LIBDEPS_DIR"], env["PIOENV"], "TFT_eSPI")
    
    # Если библиотека ещё не скачана, ждём
    if not os.path.exists(dst_dir):
        print("TFT_eSPI not yet downloaded, skipping...")
        return
    
    dst = os.path.join(dst_dir, "User_Setup.h")
    
    if os.path.exists(src):
        shutil.copy2(src, dst)
        print(f"Copied User_Setup.h to {dst}")
    else:
        print(f"Source not found: {src}")

# Регистрируем callback
Import("env")
env.AddPreAction("build", copy_user_setup)
