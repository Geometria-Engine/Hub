start Geometria

class Project:
	name = "Hub"
	version = "0.1"
	cmd = "hub"

# Description.

def hello():
	print(f"""

Welcome to Hub! (v{Project.version})

To get started, run:

	"{Project.cmd} clone"

to start contributing!

		""")

end_safezone()

def cmd(command):
	if command == "clone":
		print("Cloning Geometria project...")