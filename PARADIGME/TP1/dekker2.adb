with TEXT_IO; use TEXT_IO;
procedure Dekker2 is
package int_io is new Integer_io(integer);
use int_io;

P1inside:boolean:=FALSE;
P2inside:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;

begin
  for I in 1..10 loop
    -- attente active:
    while (P2inside) loop
      Put_Line("Process1 en attente");
    end loop;
    P1inside:=TRUE;
    -- <SC>
    Put_Line ("processus1 en SC");
    -- <SC>
    -- protocole de sortie:
    P1inside:=FALSE;
  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
  for J in 1..10 loop
  -- attente active:
  while (P1inside) loop
    Put_Line("Process2 en attente");
  end loop;
  P2inside:=TRUE;
    -- <SC>
  Put_Line ("processus2 en SC");
    -- <SC>
  -- protocole de sortie:
  P2inside:=FALSE;
  end loop;
end processus2;

begin
  Null;
end Dekker2;
