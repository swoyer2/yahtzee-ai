import subprocess
from pathlib import Path

VERSION = "v2"

for i in range(400):
    directory_path = Path(f"../{VERSION}")
    file_path = directory_path / f"sim_{i}.txt" 
    if (file_path).is_file():
        print(f"Skipping sim_{i}")
        continue
    else:
        result = subprocess.run(['./main'], capture_output=True, text=True, check=True)
        with open(f"../{VERSION}/sim_{i}.txt", "w") as f:
            f.write(result.stdout.strip())


