#pragma once
class FieldClassification
{
public:
	
	static vector<CategoriesData>* GenerateCategories(CStringW fieldName, FieldType fieldType, vector<VARIANT*>& srcValues,
		tkClassificationType ClassificationType, long numClasses, long& errorCode);
	static void GetMinValue(vector<VARIANT*>& srcValues, CComVariant& result, bool min);
	
};

