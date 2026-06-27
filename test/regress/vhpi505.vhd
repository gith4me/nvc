library ieee;
use ieee.std_logic_1164.all;

entity vhpi505 is
end entity;

architecture test of vhpi505 is
    -- Note: no signals declared here.  The VHPI plugin looks up the
    -- std_logic type by name and creates a signal of that type.
begin

    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
