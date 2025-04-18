import matplotlib.pyplot as plt
import uproot
import awkward as ak
import numpy as np

pulse = uproot.open("../MergedData/check_merged_08_02.2025.root")["pulse"].arrays(["channel", "time"], entry_start=0, entry_stop = 10)

fig, ax = plt.subplots( nrows=1, ncols=1 )  # create figure & 1 axis
ax.plot(pulse["time"][0][0]*1e9, pulse["channel"][0][2])
plt.title("The signal from the MCP")
plt.xlabel("Time [ns]")
plt.ylabel("Amplitude [V]")
plt.show()
fig.savefig("mcp.png")
plt.close(fig)

fig, ax = plt.subplots( nrows=1, ncols=1 )  # create figure & 1 axis
ax.plot(pulse["time"][0][0]*1e9, pulse["channel"][0][7])
plt.title("The signal from the acquisition board")
plt.xlabel("Time [ns]")
plt.ylabel("Amplitude [V]")
plt.show()
fig.savefig("clock.png")
plt.close(fig)


