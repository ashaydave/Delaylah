classdef DelayLah < audioPlugin
    properties
        delayTime = 0.013; % Delay length in seconds
        modulationRate = 1.00; % Modulation rate in Hz
        modulationDepth = 0.003; % Modulation depth in seconds
        feedback = 0.3; % Feedback as a percentage
        dryWetMix = 0.4; % Dry/Wet balance [0 for all dry, 1 for all wet]
    end

    properties (Access = private)
        delayBufferLeft; % Delay line buffer for the left channel
        delayBufferRight; % Delay line buffer for the right channel
        modulationDepthSamples; % Variable to store modulation depth samples
        delayLengthSamples;
    end

    properties (Constant)
        PluginInterface = audioPluginInterface(...
            audioPluginParameter('delayTime', 'Label', 's', 'Mapping',{'lin',0.01, 0.05}, 'Style', 'rotaryknob', 'Layout', [3,1], 'DisplayName', 'Delay Time', 'DisplayNameLocation', 'Above'),...
            audioPluginParameter('modulationRate', 'Label', 'Hz', 'Mapping',{'lin',0.1, 5}, 'Style', 'rotaryknob', 'Layout', [3,2], 'DisplayName', 'Mod Rate', 'DisplayNameLocation', 'Above'), ...
            audioPluginParameter('modulationDepth', 'Label', 's', 'Mapping',{'lin',0.003, 0.025}, 'Style', 'rotaryknob', 'Layout', [3,3], 'DisplayName', 'Depth', 'DisplayNameLocation', 'Above'), ...
            audioPluginParameter('feedback', 'Label', '%', 'Mapping',{'lin',0, 0.9}, 'Style', 'rotaryknob', 'Layout', [3,4], 'DisplayName', 'Feedback', 'DisplayNameLocation', 'Above'), ...
            audioPluginParameter('dryWetMix', 'Mapping',{'lin', 0, 1}, 'Style', 'rotaryknob', 'Layout', [3,5], 'DisplayName', 'Mix', 'DisplayNameLocation', 'Above'), ...
            audioPluginGridLayout('RowHeight', [20, 20, 160, 120, 160], 'ColumnWidth', [100, 100, 100, 100, 100]));
    end

    methods
        function reset(obj)
            % Initialize the chorus effect when audio plugin is reset
            sampleRate = getSampleRate(obj);
            obj.delayLengthSamples = round(obj.delayTime * sampleRate);
            obj.modulationDepthSamples = round(obj.modulationDepth * sampleRate);

            obj.delayBufferLeft = zeros(obj.delayLengthSamples + obj.modulationDepthSamples, 1);
            obj.delayBufferRight = zeros(obj.delayLengthSamples + obj.modulationDepthSamples, 1);
        end

        function output = process(obj, input)
            % Apply the chorus effect to the input audio for both left and right channels
            output = zeros(size(input));
            modulationWave = 2 * pi * obj.modulationRate / getSampleRate(obj);
            modulatedOutputLeft = zeros(size(input, 1), 1);
            modulatedOutputRight = zeros(size(input, 1), 1);

            for i = 1:length(input)
                modulatedSample = obj.modulationDepthSamples * sin(modulationWave * i);
                modulatedSample = modulatedSample + obj.delayLengthSamples;

                interp_y1_left = obj.delayBufferLeft(max(1, floor(modulatedSample)));
                interp_y2_left = obj.delayBufferLeft(min(length(obj.delayBufferLeft), ceil(modulatedSample)));
                query_sample_left = modulatedSample - floor(modulatedSample);

                modulatedOutputLeft(i) = interp_y1_left + (interp_y2_left - interp_y1_left) * query_sample_left;

                interp_y1_right = obj.delayBufferRight(max(1, floor(modulatedSample)));
                interp_y2_right = obj.delayBufferRight(min(length(obj.delayBufferRight), ceil(modulatedSample)));
                query_sample_right = modulatedSample - floor(modulatedSample);

                modulatedOutputRight(i) = interp_y1_right + (interp_y2_right - interp_y1_right) * query_sample_right;

                newSampleLeft = (input(i, 1) + modulatedOutputLeft(i) * obj.feedback);
                newSampleRight = (input(i, 2) + modulatedOutputRight(i) * obj.feedback);

                obj.delayBufferLeft = [newSampleLeft; obj.delayBufferLeft(1 : length(obj.delayBufferLeft) - 1)];
                obj.delayBufferRight = [newSampleRight; obj.delayBufferRight(1 : length(obj.delayBufferRight) - 1)];

                output(i, :) = [newSampleLeft, newSampleRight]; %; newSampleRight
            end

            output = (1 - obj.dryWetMix) * input + obj.dryWetMix * output;
        end
    end
end
