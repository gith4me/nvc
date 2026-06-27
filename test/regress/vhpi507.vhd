library ieee;
use ieee.std_logic_1164.all;

entity vhpi507 is
end entity;

architecture test of vhpi507 is
    -- No signals.  The VHPI plugin constructs a constrained
    -- std_logic_vector subtype and a record type and creates signals of
    -- them.
begin

    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
