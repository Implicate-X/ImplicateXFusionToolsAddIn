class SketchTexts : public core::Base {  
public:  

   /// Function that returns the specified sketch text using an index into the collection.  
   /// index : The index of the item within the collection to return. The first item in the collection has an index of 0.  
   /// Returns the specified item or null if an invalid index was specified.  
   core::Ptr<SketchText> item(size_t index) const;  

   /// Returns the number of texts in the sketch.  
   size_t count() const;  

   /// Adds support for range-based for loops by providing begin() and end() methods.  
   auto begin() const { return Iterator(this, 0); }  
   auto end() const { return Iterator(this, count()); }  

   typedef SketchText iterable_type;  
   template <class OutputIterator> void copyTo(OutputIterator result);  

   ADSK_FUSION_SKETCHTEXTS_API static const char* classType();  
   ADSK_FUSION_SKETCHTEXTS_API const char* objectType() const override;  
   ADSK_FUSION_SKETCHTEXTS_API void* queryInterface(const char* id) const override;  
   ADSK_FUSION_SKETCHTEXTS_API static const char* interfaceId() { return classType(); }  

private:  

   // Raw interface  
   virtual SketchText* item_raw(size_t index) const = 0;  
   virtual size_t count_raw() const = 0;  

   // Iterator class for range-based for loop support.  
   class Iterator {  
   public:  
       Iterator(const SketchTexts* collection, size_t index) : _collection(collection), _index(index) {}  
       bool operator!=(const Iterator& other) const { return _index != other._index; }  
       void operator++() { ++_index; }  
       core::Ptr<SketchText> operator*() const { return _collection->item(_index); }  

   private:  
       const SketchTexts* _collection;  
       size_t _index;  
   };  
};
