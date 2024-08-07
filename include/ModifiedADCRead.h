#include <Arduino.h>
class ModifiedADC
{
	private:
		inline double interpolation_function(double x);
	public:
		ModifiedADC();
		~ModifiedADC();
		double readVolts(uint8_t pin, uint16_t tolerance = 10);
		uint16_t readMilliVolts(uint8_t pin, uint16_t tolerance = 10);
		uint16_t read(uint8_t pin, uint16_t tolerance = 10);
};

ModifiedADC::ModifiedADC()
{

}

ModifiedADC::~ModifiedADC()
{

}

double ModifiedADC::interpolation_function(double x)
{
	if((56.0217 <= x) && (x <= 72.4706))
	{
		return (0.0007781675370389513 * x + 0.15780573169026496);
	}
	else if((72.4706 <= x) && (x <= 83.8333))
	{
		return (0.0009064746935147467 * x + 0.1485072350761702);
	}
	else if((83.8333 <= x) && (x <= 102.727))
	{
		return (0.0007304021975579161 * x + 0.16326797345146796);
	}
	else if((102.727 <= x) && (x <= 114.786))
	{
		return (0.0009868148271000886 * x + 0.13692747325648918);
	}
	else if((114.786 <= x) && (x <= 186.975))
	{
		return (0.000849159844297608 * x + 0.15272833811245473);
	}
	else if((186.975 <= x) && (x <= 302.0))
	{
		return (0.0008085198869810913 * x + 0.16032699413171045);
	}
	else if((302.0 <= x) && (x <= 445.027))
	{
		return (0.0009438777293797672 * x + 0.11944892572731036);
	}
	else if((445.027 <= x) && (x <= 522.162))
	{
		return (0.0007259998703571662 * x + 0.2164104556945614);
	}
	else if((522.162 <= x) && (x <= 658.538))
	{
		return (0.0008271250073326687 * x + 0.16360675192115903);
	}
	else if((658.538 <= x) && (x <= 772.081))
	{
		return (0.0008507790000264209 * x + 0.1480296988806009);
	}
	else if((772.081 <= x) && (x <= 890.379))
	{
		return (0.0008039020101776875 * x + 0.18422253208000083);
	}
	else if((890.379 <= x) && (x <= 1000.75))
	{
		return (0.0008380824673147842 * x + 0.15378897083472975);
	}
	else if((1000.75 <= x) && (x <= 1144.84))
	{
		return (0.0007530015962245817 * x + 0.23893365257824983);
	}
	else if((1144.84 <= x) && (x <= 1263.45))
	{
		return (0.000868392209763088 * x + 0.1068298625748263);
	}
	else if((1263.45 <= x) && (x <= 1388.89))
	{
		return (0.0007892219387755097 * x + 0.2068575414540822);
	}
	else if((1388.89 <= x) && (x <= 1502.31))
	{
		return (0.0008728619291130322 * x + 0.09069079527420043);
	}
	else if((1502.31 <= x) && (x <= 1628.38))
	{
		return (0.0007535496152930917 * x + 0.2699348774490353);
	}
	else if((1628.38 <= x) && (x <= 1745.14))
	{
		return (0.0008907159986296666 * x + 0.0465758821514235);
	}
	else if((1745.14 <= x) && (x <= 1883.33))
	{
		return (0.0007236413633403301 * x + 0.33814451118025635);
	}
	else if((1883.33 <= x) && (x <= 1996.51))
	{
		return (0.0008482063968899082 * x + 0.10354744654532921);
	}
	else if((1996.51 <= x) && (x <= 2120.23))
	{
		return (0.0008891044293566123 * x + 0.021894115745229925);
	}
	else if((2120.23 <= x) && (x <= 2248.4))
	{
		return (0.0008348287430756004 * x + 0.13697105406881982);
	}
	else if((2248.4 <= x) && (x <= 2353.37))
	{
		return (0.0008383347623130441 * x + 0.12908812041535134);
	}
	else if((2353.37 <= x) && (x <= 2483.89))
	{
		return (0.0008197977321483315 * x + 0.1727126110940811);
	}
	else if((2483.89 <= x) && (x <= 2604.22))
	{
		return (0.0007978060334081289 * x + 0.22733757167788293);
	}
	else if((2604.22 <= x) && (x <= 2741.98))
	{
		return (0.0007041231126596968 * x + 0.4713085075493648);
	}
	else
	{
		return 0.000822927926317303* x + 0.16071424188182937;
	}
}

double ModifiedADC::readVolts(uint8_t pin, uint16_t tolerance)
{
	uint32_t data[100] = {0};
	for (int i = 0; i < 100; i++)
	{
		data[i] = analogRead(pin);
	}
	std::sort(data, data + 100);
	int max = 0;
	int left = -1;
	int right = -1;
	for (int i = 0; i < 10; i++)
    {
        uint32_t* it = std::upper_bound(data + i, data + 100, data[i] + tolerance);
        if(it - data - i > max)
        {
            max = it - data - i;
            left = i;
            right = it - data - 1;
        }
    }
	uint32_t sum = 0;
	for (int i = left; i <= right; i++)
	{
		sum += data[i];
	}
	return interpolation_function(double(sum / (right - left + 1)));
}

uint16_t ModifiedADC::readMilliVolts(uint8_t pin, uint16_t tolerance)
{
	return uint16_t(readVolts(pin, tolerance) * 1000);
}

uint16_t ModifiedADC::read(uint8_t pin, uint16_t tolerance)
{
	return uint16_t(readVolts(pin, tolerance) * 4095 / 3.3);
}