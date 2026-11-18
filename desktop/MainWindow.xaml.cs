using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
namespace CircuitDesigner;
public partial class MainWindow : Window {
 public MainWindow(){InitializeComponent();}
 private void Add_Click(object sender,RoutedEventArgs e){
  if(!int.TryParse(NodeA.Text,out var a)||!int.TryParse(NodeB.Text,out var b)||a<0||b<0||a==b||!double.TryParse(ValueBox.Text,out var val)||!double.IsFinite(val)){
   MessageBox.Show("Enter distinct nonnegative nodes and a valid numeric value.");return;
  }
  string type=((ComboBoxItem)PartType.SelectedItem).Content.ToString()=="Resistor"?"R":"V";
  if(type=="R"&&val<=0){MessageBox.Show("Resistance must be positive");return;}
  string name=NameBox.Text.Trim().Replace(" ","_");
  if(string.IsNullOrWhiteSpace(name)){MessageBox.Show("Enter an element name");return;}
  Parts.Items.Add($"{type} {name} {a} {b} {val:System.Globalization.CultureInfo.InvariantCulture}");
 }
 private string Netlist()=>string.Join(Environment.NewLine,Parts.Items.Cast<string>());
 private void Export_Click(object sender,RoutedEventArgs e){
  var dialog=new Microsoft.Win32.SaveFileDialog{Filter="Netlist files|*.txt",FileName="circuit.txt"};
  if(dialog.ShowDialog()==true)File.WriteAllText(dialog.FileName,Netlist()+Environment.NewLine);
 }
 private void Solve_Click(object sender,RoutedEventArgs e){
  if(Parts.Items.Count==0){Output.Text="Add components first.";return;}
  var path=Path.Combine(Path.GetTempPath(),"circuit_"+Guid.NewGuid().ToString("N")+".txt");
  try{
   File.WriteAllText(path,Netlist());
   var executable=Path.Combine(AppContext.BaseDirectory,"circuit_solver.exe");
   if(!File.Exists(executable)){Output.Text="Build circuit_solver.exe from the C++ solver and place it beside the desktop executable.";return;}
   var p=new Process{StartInfo=new ProcessStartInfo(executable){ArgumentList={path},RedirectStandardOutput=true,RedirectStandardError=true,UseShellExecute=false,CreateNoWindow=true}};
   p.Start();string stdout=p.StandardOutput.ReadToEnd(),stderr=p.StandardError.ReadToEnd();p.WaitForExit();Output.Text=p.ExitCode==0?stdout:stderr;
  }catch(Exception ex){Output.Text=ex.Message;}finally{File.Delete(path);}
 }
}