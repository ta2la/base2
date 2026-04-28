Returns the entire CmdExeRecCol log as a single JSON-wrapped string.

Usage: `log_dump`

Returns one arg `--LOG {"cmd":"[0] ...\n[1] ...\n..."}` — all executed command records joined by newlines, each prefixed with `[index]`, then wrapped in a compact JSON object under key `cmd` so the value is safe for transport.
