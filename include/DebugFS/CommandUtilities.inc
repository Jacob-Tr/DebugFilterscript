getStrParam(const params[], &index, dest[255]) {dest = (strlen(params) > index) ? strtok(params, index) : "";}

bool:getNumericParam(const params[], &index, &dest)
{
	new param[255];
	getStrParam(params, index, param);

	if(!isnumeric(param)) return false;

	dest = strval(param);
	return true;
}
