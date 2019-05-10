with TEXT_IO; use TEXT_IO;
procedure Dekker3 is
package int_io is new Integer_io(integer);
use int_io;

P1Wantstoenter:boolean:=FALSE;
P2Wantstoenter:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;

begin
  for I in 1..10 loop
    -- protocole d'entree:
    P1Wantstoenter:=TRUE;
    -- attente active:
    while (P2Wantstoenter) loop
      Put_Line("Process1 en attente");
    end loop;
    -- <SC>
  Put_Line ("processus1 en SC");
    -- <SC>
  -- protocole de sortie:
  P1Wantstoenter:=FALSE;
  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
  for I in 1..10 loop
    -- protocole d'entree:
    P2Wantstoenter:=TRUE;
    -- attente active:
    while (P1Wantstoenter) loop
      Put_Line("Process2 en attente");
    end loop;
    -- <SC>
    Put_Line ("processus2 en SC");
    -- <SC>
    -- protocole de sortie:
    P2Wantstoenter:=FALSE;
  end loop;
end processus2;

begin
  Null;
end Dekker3;
