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

// class join
// {
// 	private:
// 		/* data */
// 	public:
// 		join(/* args */);
// 		virtual	~join();

// 		int	execute(void);
// };

