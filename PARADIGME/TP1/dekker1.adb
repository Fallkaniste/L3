with TEXT_IO; use TEXT_IO;
procedure Dekker1 is
package int_io is new Integer_io(integer);
use int_io;

processN:integer:=1;

task processus1;
task body processus1 is

I:integer:=0;

begin
  for I in 1..10 loop
    -- attente active:
    while (processN=2) loop
      Put_Line("Process1 en attente");
    end loop;
    -- <SC>
    Put_Line ("processus1 en SC");
    -- <SC>
    -- protocole de sortie:
    processN:=2;
  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
  for J in 1..10 loop
  -- attente active:
    while (processN=1) loop
      Put_Line ("processus2 en attente");
    end loop;
    -- <SC>
    Put_Line ("processus2 en SC");
    -- <SC>
    -- protocole de sortie:
    processN:=1;
  end loop;
end processus2;


begin
  Null;
end Dekker1;
