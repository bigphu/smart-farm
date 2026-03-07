Import("env")  # type: ignore

try:
  with open(".env", "r") as f:
    lines = f.readlines()
    
    for line in lines:
      line = line.strip()
      # Ignore blank lines and comments
      if not line or line.startswith("#"): 
        continue
        
      key, value = line.split("=", 1)
      
      # Inject the key-value pair into the C++ compiler as a macro.
      env.Append(CPPDEFINES=[(key, f'\\"{value}\\"')])  # type: ignore
      
except FileNotFoundError:
  print("WARNING: No .env file found. C++ might fail to compile if secrets are missing.")