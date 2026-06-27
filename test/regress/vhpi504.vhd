entity vhpi504 is
end entity;

architecture test of vhpi504 is
    signal x : real := 0.0;
begin

    -- The VHPI plugin creates a new real signal, drives it, and ends
    -- the simulation.  This process is just a watchdog.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
