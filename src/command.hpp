#pragma once

class command
{
	private:
		/* data */
	public:
		command(/* args */);
		virtual	~command();

		virtual	int	execute(void) = 0;
};

