library ieee;
use ieee.std_logic_1164.all;

entity vhpi510 is
end entity;

architecture test of vhpi510 is
    type rec_t is record
        a : std_logic;
        b : std_logic_vector;
        c : bit_vector;
    end record;
    subtype sub_t is rec_t(b(7 downto 0), c(1 to 3));
    signal r : sub_t;
begin

    -- The VHPI plugin creates a new signal of the record subtype, whose
    -- array fields are constrained by the subtype rather than the record
    -- type, drives its fields, and ends the simulation.  This process is
    -- a watchdog.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
