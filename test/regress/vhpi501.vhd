library ieee;
use ieee.std_logic_1164.all;

entity vhpi501 is
end entity;

architecture test of vhpi501 is
    signal x : std_logic := '0';
begin

    -- The VHPI plugin creates a new signal, drives it, and ends the
    -- simulation.  This process is just a watchdog.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
