import serial
import json
import time
import os
import keyboard
from dotenv import load_dotenv

from rich.live import Live
from rich.panel import Panel
from rich.layout import Layout
from rich.align import Align
from rich.text import Text
from rich.rule import Rule
from rich.console import Group

load_dotenv()

# Configure your ESP32 Serial Port here!
SERIAL_PORT = os.getenv("SERIAL_PORT", "COM3")
BAUD_RATE = 115200

TREE_ASCII = """
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⠟⠻⢶⣀⣠⣽⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣠⡀⠀⠀⠀⢿⣅⠀⠀⠀⠀⠉⠉⠹⣧⠀⠀⣀⣀⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠴⠟⠉⠉⠛⣷⡤⠴⢾⣁⡀⠀⠀⠀⠀⠀⠀⠹⡧⠛⠉⠛⠛⢷⣤⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣧⠀⠀⠀⠀⠀⠉⠀⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡼⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣀⣀⣀⡀⠀⠀⠈⢻⣆⣤⣀⣠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢶⡟⢹⣧⣀⡄⢀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢀⣠⡴⠟⠈⠉⠙⠷⠶⣶⣆⠀⣠⣽⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣠⣤⣤⠶⣶⡟⢁⣼⠁⠉⠓⠚⠉⣿⣀⠀⠀⠀⠀⠀⠀⠀
⢀⣾⠛⠁⠀⠀⠀⠀⠀⠀⠀⠈⠛⠚⠁⠀⠀⠀⠀⠀⠀⣀⠀⠀⢀⡼⠋⠀⠀⠁⠀⠀⠀⠙⠉⠀⠀⠀⠀⠀⠀⢀⡽⠀⠀⠀⠀⠀⠀⠀
⢾⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⢸⣳⣼⣿⢠⣰⡟⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠿⢴⣖⠀⠀⠀⠀⠀
⢀⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠋⠿⢾⡆⠀⢳⣿⠈⡷⢯⠳⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠀⠀⠀⠀⠀
⢸⣿⡀⠀⢀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣾⠿⡄⢀⠎⢘⠛⡿⢧⣤⣤⣦⠄⠀⣤⡄⠀⠀⠀⠀⢠⣤⢤⡤⣞⡥⣶⣄⣀⠀⠀
⠀⠸⣷⣤⣾⣷⣀⣀⣿⣷⣠⣴⣤⡀⠀⠐⠒⠋⠀⠈⠳⣷⢤⡀⡎⢸⠁⠀⠈⡇⡍⠀⢠⣷⣷⣂⡤⠄⠀⠈⠛⠳⠷⠟⠀⠀⠀⠘⠋⣿
⠀⠀⠀⠀⠀⠈⠛⢿⣯⡉⠉⣠⠏⠀⠐⣶⠆⠀⢀⣠⣤⡌⢲⠙⠃⣾⠀⠀⡼⣰⠓⢊⣡⠷⢯⡑⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿
⠀⠀⠀⠀⢀⣀⣠⡼⠉⠙⠷⠃⠀⠀⡴⢡⠔⠚⠉⠀⠘⣿⡠⠖⢻⢮⠀⠀⠀⠀⣾⣩⣀⣄⣀⡱⠘⠓⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸
⠀⠀⠀⠰⡿⠍⠁⠀⠀⠀⠀⠀⠀⠈⠴⠃⠀⠀⠀⠀⠀⠈⠀⠀⠀⢛⡆⢸⢨⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡴⠉
⠀⠀⠀⠀⣧⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⣠⠞⠁⠃⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢴⣶⢞⠛⠛⠃⠀⠀
⠀⠀⠀⠀⠈⠙⠛⣷⣠⠀⠀⠀⢀⣷⣀⠀⠀⠀⠀⢠⣤⠾⠚⠛⣧⠀⠀⢀⡼⢦⣴⠛⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⢛⡿⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠈⠿⠷⠶⠾⠻⡭⠟⠀⠀⠀⠀⢠⣉⣷⠀⠀⡏⠀⠀⣞⠀⠀⠀⠸⣏⡀⠀⠀⠀⠀⠀⠀⡶⠶⠶⠒⠾⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⠴⠶⠴⠴⠷⠾⠉⠁⠀⢠⡇⠀⠀⢸⠀⠀⠀⠀⠈⠙⠛⣦⣠⠤⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣇⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⣼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠋⠀⠀⠀⠳⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"""

def get_text_bar(value, max_val=100, width=15):
  """Generates a visual progress bar string based on a 0-100 scale."""
  try:
    val = max(0, min(max_val, int(value)))
  except (ValueError, TypeError):
    val = 0
  filled = int((val / max_val) * width)
  # FIX: Removed the '[' and ']' around the bar to prevent Markup crashes!
  return f"[{'█' * filled}{'░' * (width - filled)}]"

def generate_ui(data):
  """Generates the Rich UI layout based on the latest JSON data"""
  if not data:
    return Panel(Align.center(Text("Waiting for ESP32 Telemetry...", style="blink yellow")), title="󰚋 Smart Farm System Monitor")

  # Extract values
  temp = float(data.get("temp", 0))
  humid = float(data.get("humid", 0))
  soil = int(data.get("soil", 0))
  raw_soil = int(data.get("rawSoil", 0))
  light = int(data.get("light", 0))
  raw_light = int(data.get("rawLight", 0))
  pump_on = data.get("pump", False)
  valid = data.get("valid", False)
  
  # 1. Top Navbar
  mqtt_status = "󰄬 CONNECTED" if data.get("mqtt") else "󰅖 DISCONNECTED"
  valid_status = "󰄴 SENSORS OK" if valid else "󰅙 SENSOR ERROR"
  nav_text = f" 󰤨 WiFi RSSI: {data.get('rssi', 0)} dBm  |  󰒘 MQTT: {mqtt_status}  |  󱎫 Uptime: {data.get('uptime', 0)}s  |  {valid_status} "
  nav_panel = Panel(Align.center(Text(nav_text, style="dim bold white")), height=3)

  # 2. Left Panel
  # Split the text into top and bottom sections to place the Rule in the middle
  left_top = f"\n\n\n[bold red]󰔏  Temperature[/]\n[bold white]{temp} [bold red]°C[/]\n\n"
  left_bottom = f"\n\n[bold cyan]󰖌  Air Humidity[/]\n[bold white]{humid} [bold cyan]%[/]\n\n"
  
  # Group them together with a Rule using your exact character and color
  left_group = Group(
    Align.center(Text.from_markup(left_top, justify="center")),
    Rule(characters="─", style="dim white"),
    Align.center(Text.from_markup(left_bottom, justify="center"))
  )
  left_panel = Panel(left_group, title="[green]Climate Stats[/]", border_style="green")

  # 3. Right Panel (Pump & Soil)
  pump_style = "[bold white]ON[/]" if pump_on else "[bold dim white]OFF[/]"
  soil_bar = get_text_bar(raw_soil, max_val=100)
  
  ui_button = "[bold white on red] 󰌌  Press 'P' to Toggle Pump [/]"
  
  # Split the text into top and bottom sections
  right_top = f"\n\n[bold cyan]󰒓  Water Pump[/]\n{pump_style}\n\n{ui_button}\n\n"
  right_bottom = f"\n\n[bold cyan]󰇧  Soil Moisture[/]\n[bold white]{soil} [bold cyan]%[/]\n\n󰎡  [dim]Raw Soil (0% - 100%)[/]\n[bold cyan]{soil_bar} [white]{raw_soil} [bold cyan]󰏉[/]\n\n"
  
  # Group them together with the Rule
  right_group = Group(
    Align.center(Text.from_markup(right_top, justify="center")),
    Rule(characters="", style="dim white"),
    Align.center(Text.from_markup(right_bottom, justify="center"))
  )
  right_panel = Panel(right_group, title="[green]Irrigation Stats[/]", border_style="green")

  # 4. Center Panels (Split into Tree and Light)
  tree_panel = Panel(Align.center(Text(TREE_ASCII, style="bold green", justify="center")), title="[green]Smart Tree[/]", border_style="green")
  
  light_bar = get_text_bar(raw_light, max_val=100)
  light_content = f"\n[bold yellow]󰖨 Growing Day:[/] {light} [bold yellow]lx[/]\n\n󰎡 [dim]Raw Light (0% - 100%):[/] [bold yellow]{light_bar} [white]{raw_light} [bold yellow]󰏉[/]\n"
  light_panel = Panel(Align.center(Text.from_markup(light_content, justify="center")), title="[yellow]Light Monitor[/]", border_style="yellow")

  # 5. Build Final Layout
  layout = Layout()
  layout.split_column(
    Layout(nav_panel, name="header", size=3),
    Layout(name="body")
  )
  
  center_layout = Layout(name="center", ratio=2)
  center_layout.split_column(
    Layout(tree_panel, name="tree_view", ratio=3),
    Layout(light_panel, name="light_view", ratio=1)
  )
  
  layout["body"].split_row(
    Layout(left_panel, name="left", ratio=1),
    center_layout,
    Layout(right_panel, name="right", ratio=1)
  )
  
  return layout

# --- Main Application Loop ---
if __name__ == "__main__":
  try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2) 
  except Exception as e:
    print(f"Failed to connect to {SERIAL_PORT}: {e}")
    exit(1)

  latest_data = {}

  with Live(generate_ui(latest_data), refresh_per_second=4, screen=True) as live:
    try:
      while True:
        # Keyboard input handling
        if keyboard.is_pressed('p'):
          try:
            ser.write(b"PUMP_TOGGLE\n") 
          except Exception:
            pass
          time.sleep(0.3) 

        # Handling ESP32 Data
        if ser.in_waiting > 0:
          try:
            # Decode and find the start of the JSON in case there is garbage data
            raw_line = ser.readline().decode('utf-8', errors='ignore').strip()
            
            start_idx = raw_line.find("{")
            if start_idx != -1:
              json_str = raw_line[start_idx:]
              latest_data = json.loads(json_str)
              live.update(generate_ui(latest_data))
          except json.JSONDecodeError:
            pass 
          except UnicodeDecodeError:
            pass
          except Exception as e:
            # FIX: Safety net! If the UI ever crashes again, it prints to the screen instead of silently freezing!
            live.update(Panel(Align.center(Text(f"UI CRASHED: {str(e)}", style="bold red"))))
        
        time.sleep(0.05)
        
    except KeyboardInterrupt:
      pass 
      
  print("Dashboard closed.")