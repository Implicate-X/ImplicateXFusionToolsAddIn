#pragma once

namespace implicatex {
	namespace fusion {
		class LanguageDropDownControl :
			public DropDownControl
		{
		public:
			/// <summary>
			/// <para>The LanguageDropDownControl class is responsible for creating a dropdown menu</para>
			/// <para>for selecting countries, populating it with command buttons for each region,</para>
			/// <para>and logging any failures encountered during the process.</para></summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool initialize(bool doTerminate = true);

			/// <summary>
			/// <para>The terminate function is responsible for deleting command definitions associated</para>
			/// <para>with region commands based on the current locale in the user interface.</para>
			/// </summary>
			void terminate();

			/// <summary>
			/// <para>The initializeCommandDefCounters function initializes a map of command definition counters</para>
			/// <para>for various languages, setting their initial values to zero if the map is empty.</para>
			/// </summary>
			void initializeCommandDefCounters();

			/// <summary>
			/// <para>The generateUniqueCommandId function generates a unique command identifier string</para>
			/// <para>based on the provided localeId, incrementing a counter associated with that locale.</para>
			/// </summary>
			///
			/// <param name="localeId">
			/// <para>localeId is a constant reference to a std::string</para>
			/// <para>that likely represents a unique identifier for a specific locale in a program.</para>
			/// </param>
			///
			/// <returns>The unique command identifier.</returns>
			std::string generateUniqueCommandId(const std::string& localeId);

			/// <summary>
			/// <para>The getCurrentCommandId function generates a unique command ID string</para>
			/// <para>based on the provided locale identifier and a counter associated with that locale.</para>
			/// </summary>
			///
			/// <param name="localeId">
			/// <para>localeId is a constant reference to a std::string</para>
			/// <para>that likely represents a unique identifier for a specific locale in a program.</para>
			/// </param>
			///
			/// <returns>The current command identifier.</returns>
			std::string getCurrentCommandId(const std::string& localeId);

		private:
			/// <summary>
			/// <para>commandDefCounters_ is a static member of the LanguageDropDownControl class,</para>
			/// <para>which stores a mapping of command definitions to their corresponding integer counters using a std::map.</para>
			/// </summary>
			static std::map<std::string, int> commandDefCounters_;
		};
	}
}
