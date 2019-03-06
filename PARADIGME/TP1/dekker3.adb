with TEXT_IO; use TEXT_IO;

--sauvegarder ce fichier sous le nom de: Dekker1.adb, et pour compiler lancer: gnatmake Dekker1.adb -o Dekker1

procedure Dekker3 is
package int_io is new Integer_io(integer);
use int_io;

P1Wantstoenter:boolean:=FALSE;
P2Wantstoenter:boolean:=FALSE;

task processus1;
task body processus1 is

I:integer:=0;


begin

  --pour tester je prend 2 iterations par exemple et non pas une boucle infinie

  for I in 1..2 loop
    -- protocole d entree
    P1Wantstoenter:=TRUE;
    while (P2Wantstoenter) loop
      Put_Line("Process1 en attente");
    end loop;

  -- process1 entre en SC
  Put_Line ("processus1 en SC");

  -- protocole de sortie
  P1Wantstoenter:=FALSE;

  end loop;
end processus1;


task processus2;
task body processus2 is

J:integer:=0;

Begin
--pour tester je prend 2 iterations et non pas une boucle infinie

for I in 1..2 loop
  -- protocole d entree
  P2Wantstoenter:=TRUE;
  while (P1Wantstoenter) loop
    Put_Line("Process2 en attente");
  end loop;

-- process1 entre en SC
Put_Line ("processus2 en SC");

-- protocole de sortie
P2Wantstoenter:=FALSE;

end loop;
end processus2;


begin
  Null;
end Dekker3;
