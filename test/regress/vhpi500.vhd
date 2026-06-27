entity vhpi500 is
end entity;

architecture test of vhpi500 is
    signal x : natural := 0;
begin

    -- The VHPI plugin creates a new signal during elaboration and ends
    -- the simulation once it has verified it.  This process is just a
    -- watchdog in case the plugin does not run.
    p1: process is
    begin
        wait for 1 ms;
        report "VHPI plugin did not end simulation" severity failure;
    end process;

end architecture;
