with TEXT_IO; use TEXT_IO;
procedure Dekker5 is
package int_io is new Integer_io(integer);
use int_io;

tour:integer:=1;
demande1:boolean:=FALSE;
demande2:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;

begin
  for I in 1..2 loop
    -- protocole d'entree:
    demande1:=TRUE;
    tour:=2;
    --attente active:
    while (demande2=TRUE AND tour/=1) loop
      Put_Line("Process1 en attente");
    end loop;
    -- <SC>
  Put_Line ("processus1 en SC");
    -- <SC>
  -- protocole de sortie:
  demande1:=FALSE;
  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
  for I in 1..2 loop
    -- protocole d'entree:
    demande2:=TRUE;
    -- attente active:
    tour:=1;
    while (demande1=TRUE AND tour/=2) loop
      Put_Line("Process2 en attente");
    end loop;
    -- <SC>
    Put_Line ("processus2 en SC");
    -- <SC>
    -- protocole de sortie:
    demande2:=FALSE;

  end loop;
end processus2;

begin
  Null;
end Dekker5;
