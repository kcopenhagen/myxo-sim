function makeAllVideo()
%%
    fpath = uigetdir('/Users/kc32/Desktop/mnt/localtigress/myxoSim');    
    lines = readlines(fullfile(fpath,'parameters.h'));

    k6str = lines(contains(lines,'K6'));
    K6t = regexp(k6str,'\d+\.?\d*','match');
    K6 = str2double(K6t{2});

    files = dir(fullfile(fpath,"frames/"));
    if (K6 > 0.0001)
        files2 = dir(fullfile(fpath,"water/"));
    end
    del = [];
    for i = 1:numel(files)
        if (files(i).name(1) == '.') || ~contains(files(i).name,'0')
            del = [del; i];
        end
    end
    files(del) = [];
    if (K6 > 0.0001) 
        files2(del) = [];
    end

    bSstr = lines(contains(lines,'boxSize'));
    BS = regexp(bSstr,'\d+\.?\d*','match');
    boxSize = str2double(BS{1});

    wR = lines(contains(lines,'waterRadius'));
    WR = regexp(wR,'\d+\.?\d*','match');
    waterRadius = str2double(WR{1});

    wrF = lines(contains(lines,'waterRadiusFactor'));
    WRF = regexp(wrF,'\d+\.?\d*','match');
    waterRadiusFactor = str2double(WRF{1});

    cR = lines(contains(lines,'beadRadius'));
    CR = regexp(cR,'\d+\.?\d*','match');
    beadRadius = str2double(CR);

    if (K6 > 0.0001)
        v = VideoWriter(fullfile(fpath,'LaserVideo.mp4'),'MPEG-4');
    else
        v = VideoWriter(fullfile(fpath,'LaserVideo.mp4'),'MPEG-4');
    end

    v.Quality = 95;
    v.FrameRate = 30;
    open(v);

    fac = 50;

    for t = 1:numel(files)
        t
        %%
        im = zeros(boxSize*fac+2);

        cells = readmatrix(fullfile(files(t).folder,files(t).name));

        xs = cells(:,1);
        ys = cells(:,2);

        inds = sub2ind(size(im),round(ys*fac+1),round(xs*fac+1));
        im(inds) = 1;
        imdil = imdilate(im,strel('disk',round(fac/6)));
        imsm = imgaussfilt(imdil,fac/6);
        noise = imgaussfilt(rand(size(im)) * 0.5 - 0.25,fac/20);
        
        imwr = imsm + noise;
        imwr(imwr>1) = 1;
        imwr(imwr<0) = 0;
        writeVideo(v, imwr);
    end
    close(v);
end