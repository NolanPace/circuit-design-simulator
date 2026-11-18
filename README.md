# Circuit Design Simulator

A cross-language DC circuit engineering tool: **C++17 numerical solver** and **C# WPF desktop schematic interface**. Build resistor and DC voltage-source networks, export a netlist, and solve node voltages using modified nodal analysis (MNA).

## C++ CLI
```bash
cmake -S solver -B build
cmake --build build
./build/circuit_solver examples/voltage-divider.txt
ctest --test-dir build --output-on-failure
```

## C# schematic editor
On **Windows with .NET 8 SDK**, open `desktop/CircuitDesigner.csproj` and run `dotnet run --project desktop`. Add parts through the forms, select a source and destination node, export a netlist and use the Solver button (if the C++ executable is present). For Windows, put `circuit_solver.exe` in the application working directory or specify its path.

Netlist grammar: `R <name> <nodeA> <nodeB> <ohms>` and `V <name> <positiveNode> <negativeNode> <volts>`. Ground is node **0**.

This MVP handles linear steady-state resistive DC networks; it does not simulate AC, transients, diodes, or SPICE device models. Never use for safety-critical electrical design.

MIT licensed.
